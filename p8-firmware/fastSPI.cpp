#include "headers/fastSPI.h"

/*
  SPIM Structure:
  typedef struct {
  __O  uint32_t  TASKS_START;                       Start SPI transaction
  __O  uint32_t  TASKS_STOP;                        Stop SPI transaction
  __O  uint32_t  TASKS_SUSPEND;                     Suspend SPI transaction
  __O  uint32_t  TASKS_RESUME;                      Resume SPI transaction
  __IO uint32_t  EVENTS_STOPPED;                    SPI transaction has stopped
  __IO uint32_t  EVENTS_ENDRX;                      End of RXD buffer reached
  __IO uint32_t  EVENTS_END;                        End of RXD buffer and TXD buffer reached
  __IO uint32_t  EVENTS_ENDTX;                      End of TXD buffer reached
  __IO uint32_t  EVENTS_STARTED;                    Transaction started
  __IO uint32_t  SHORTS;                            Shortcut register
  __IO uint32_t  INTENSET;                          Enable interrupt
  __IO uint32_t  INTENCLR;                          Disable interrupt
  __IO uint32_t  ENABLE;                            Enable SPIM
  SPIM_PSEL_Type PSEL;                              Unspecified
  __IO uint32_t  FREQUENCY;                         SPI frequency
  SPIM_RXD_Type RXD;                                RXD EasyDMA channel
  SPIM_TXD_Type TXD;                                TXD EasyDMA channel
  __IO uint32_t  CONFIG;                            Configuration register
  __IO uint32_t  ORC;                               Over-read character. Character clocked out in case and over-read of the TXD buffer.
  } NRF_SPIM_Type;
*/

/*
  Initialize fast SPI interface for display/flash for use with EasyDMA
*/
void initFastSPI() {
  pinMode(LCD_SCK, OUTPUT);  //SPI clock (pin 2)
  pinMode(LCD_SDI, OUTPUT);  //SPI MOSI (pin 3)
  pinMode(SPI_MISO, INPUT);  //Flash memory MISO (pin 4)
  pinMode(LCD_CS, OUTPUT);   //LCD slave select

  //Make sure spi bus has well defined levels at boot (to stop garbled display)
  digitalWrite(LCD_SCK, HIGH);
  digitalWrite(LCD_SDI, HIGH);
  digitalWrite(LCD_CS, HIGH);

  NRF_SPIM2->PSEL.SCK = LCD_SCK;      //This is the same as the display SCK since SPI is a shared bus (pin 2)
  NRF_SPIM2->PSEL.MOSI = LCD_SDI;     //Ditto for the MOSI (pin 3)
  NRF_SPIM2->PSEL.MISO = SPI_MISO;    //This is only used by the flash storage (pin 4)
  NRF_SPIM2->FREQUENCY = 0x80000000;  //SPI bus speed
  NRF_SPIM2->INTENSET = 0;            //SPI interrupt disable
  NRF_SPIM2->ORC = 255;               //Over-read character
  NRF_SPIM2->CONFIG = 0;              //Configuration register
}

/*
  Enable SPI
*/
void enableSPI(bool state) {
  if (state) {
    NRF_SPIM2->ENABLE = 7;
  } else {
    while (NRF_SPIM2->ENABLE == 0) NRF_SPIM2->ENABLE = 0;
  }
}

/*
  Enable workaround for writing 1 byte (I think, don't quite know what it is doing)
*/
void enableSingleByteWorkaround(NRF_SPIM_Type *spim, uint32_t ppi_channel, uint32_t gpiote_channel) {
  NRF_GPIOTE->CONFIG[gpiote_channel] = (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos) |
                                       (spim->PSEL.SCK << GPIOTE_CONFIG_PSEL_Pos) |
                                       (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos);

  NRF_PPI->CH[ppi_channel].EEP = (uint32_t)&NRF_GPIOTE->EVENTS_IN[gpiote_channel];
  NRF_PPI->CH[ppi_channel].TEP = (uint32_t)&spim->TASKS_STOP;
  NRF_PPI->CHENSET = 1U << ppi_channel;
}

/*
  Disable workaround for writing 1 byte (again speculation)
*/
void disableSingleByteWorkaround(NRF_SPIM_Type *spim, uint32_t ppi_channel, uint32_t gpiote_channel) {
  NRF_GPIOTE->CONFIG[gpiote_channel] = 0;
  NRF_PPI->CH[ppi_channel].EEP = 0;
  NRF_PPI->CH[ppi_channel].TEP = 0;
  NRF_PPI->CHENSET = ppi_channel;
}
/*
  Write a byte buffer over SPI
*/
void writeSPI(uint8_t *ptr, uint32_t len) {
  //Handle edge case workaround
  if (len == 1)
    enableSingleByteWorkaround(NRF_SPIM2, 8, 8);
  else
    disableSingleByteWorkaround(NRF_SPIM2, 8, 8);

  int pointerOffset = 0;  //Pointer offset from first byte in buffer
  do {
    NRF_SPIM2->EVENTS_END = 0;
    NRF_SPIM2->EVENTS_ENDRX = 0;
    NRF_SPIM2->EVENTS_ENDTX = 0;

    /*
      Transmit structure
      typedef struct {
      __IO uint32_t  PTR;     Data pointer
      __IO uint32_t  MAXCNT;  Maximum number of bytes in transmit buffer
      __I  uint32_t  AMOUNT;  Number of bytes transferred in the last transaction
      __IO uint32_t  LIST;    EasyDMA list type
      } SPIM_TXD_Type;
    */

    NRF_SPIM2->TXD.PTR = (uint32_t)ptr + pointerOffset;

    if (len <= 0xFF) {              //If we have reached the end of the buffer (<=255 bytes to go)
      NRF_SPIM2->TXD.MAXCNT = len;  //Max bytes to TX = length
      pointerOffset += len;
      len = 0;
    } else {                        //Transmit in 255 byte chunks
      NRF_SPIM2->TXD.MAXCNT = 255;  //Max bytes to TX = 255
      pointerOffset += 255;         //Move pointer 255 bytes along buffer
      len -= 255;                   //Decrease lenth by 255
    }
    NRF_SPIM2->RXD.PTR = 0;
    NRF_SPIM2->RXD.MAXCNT = 0;
    NRF_SPIM2->TASKS_START = 1;         //Start SPI transaction
    while (NRF_SPIM2->EVENTS_END == 0)  //Wait until write is done
      ;
    NRF_SPIM2->EVENTS_END = 0;
  } while (len);
}

/*
  Send data in command mode
*/
void sendSPICommand(uint8_t command) {
  digitalWrite(LCD_RS, LOW);  //Put display into command receive mode
  writeSPI(&command, 1);      //Write command over SPI (requires single byte workaround)
  digitalWrite(LCD_RS, HIGH);
}

/*
  Helper function to write a single byte
*/
void writeSPISingleByte(uint8_t toWrite) {
  writeSPI(&toWrite, 1);
}

/*
  Handle enabling SPI communication and chip select
*/
void preWrite() {
  enableSPI(true);
  digitalWrite(LCD_CS, LOW);  //Chip select low (select display)
}

/*
  Handle stopping SPI and deselecting display
*/
void postWrite() {
  digitalWrite(LCD_CS, HIGH);  //Unselect
  enableSPI(false);
}
