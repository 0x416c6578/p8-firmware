#include "headers/touch.h"
/*
  The touch panel is i2c address 0x15
  The controller will go to sleep when no event is detected
  It appears disconnected, so for communication to work, the controller must be woken with a tap
  I2C on the nRF52832 also uses EasyDMA, however <Wire.h> handles everything to do with this struct: (here for reference)
  typedef struct {                                    TWIM Structure
    __O  uint32_t  TASKS_STARTRX;                     Start TWI receive sequence
    __O  uint32_t  TASKS_STARTTX;                     Start TWI transmit sequence
    __O  uint32_t  TASKS_STOP;                        Stop TWI transaction. Must be issued while the TWI master is not suspended.
    __O  uint32_t  TASKS_SUSPEND;                     Suspend TWI transaction
    __O  uint32_t  TASKS_RESUME;                      Resume TWI transaction
    __IO uint32_t  EVENTS_STOPPED;                    TWI stopped
    __IO uint32_t  EVENTS_ERROR;                      TWI error
    __IO uint32_t  EVENTS_SUSPENDED;                  Last byte has been sent out after the SUSPEND task has been issued, TWI traffic is now suspended.
    __IO uint32_t  EVENTS_RXSTARTED;                  Receive sequence started
    __IO uint32_t  EVENTS_TXSTARTED;                  Transmit sequence started
    __IO uint32_t  EVENTS_LASTRX;                     Byte boundary, starting to receive the last byte
    __IO uint32_t  EVENTS_LASTTX;                     Byte boundary, starting to transmit the last byte
    __IO uint32_t  SHORTS;                            Shortcut register
    __IO uint32_t  INTEN;                             Enable or disable interrupt
    __IO uint32_t  INTENSET;                          Enable interrupt
    __IO uint32_t  INTENCLR;                          Disable interrupt
    __IO uint32_t  ERRORSRC;                          Error source
    __IO uint32_t  ENABLE;                            Enable TWIM
    TWIM_PSEL_Type PSEL;                              Unspecified
    __IO uint32_t  FREQUENCY;                         TWI frequency
    TWIM_RXD_Type RXD;                                RXD EasyDMA channel
    TWIM_TXD_Type TXD;                                TXD EasyDMA channel
    __IO uint32_t  ADDRESS;                           Address used in the TWI transfer
  } NRF_TWIM_Type;

  Unlike the PineTime, the P8 watch uses the CST716S touch controller, rather than the CST816S
  However implementation wise, getting the data is equivalent
  As per the reference driver, waking the device can only be done by toggling the reset pin low for 20ms
*/

touchDataStruct touchData = {-1, -1, -1};

/* 
  Initialize the touch panel (basically just reset and put into running (not sleeping) state)
*/
void initTouch() {
  pinMode(TP_RESET, OUTPUT);  //Reset pin
  pinMode(TP_INT, INPUT);     //Interrupt pin

  Wire.begin();           //Initialize i2c bus
  Wire.setClock(200000);  //Set clock to 200Kbaud

  resetTouchController(true);  //Since it is at boot, we must pass in true for the extra delay
}

/* 
  Reset the touch controller
  Setting bootup to true will add an additional 50 msec high write to the reset pin (used to put pin into known state after boot)
  Calling just resetTouchController() will not add the bootup delay
 */
void resetTouchController(bool bootup) {
  //Reset touch controller:
  if (bootup) {
    digitalWrite(TP_RESET, HIGH);
    delay(50);
  }
  digitalWrite(TP_RESET, LOW);
  delay(5);
  digitalWrite(TP_RESET, HIGH);
  delay(50);
}

/* 
  Read touch data from the controller and store it in the global touchDataStruct
 */
void updateTouchStruct() {
  int readBufSize = 6;
  uint8_t readBuf[readBufSize];  //Read buffer where the raw bytes are stored
  Wire.beginTransmission(0x15);
  Wire.write(0x1);  //Probe touch display at i2c port 1 to see if the touch display is awake
  if (Wire.endTransmission()) {
    /* 
      Errors upon calling endTransmission()
      0 : Success
      1 : Data too long
      2 : NACK on transmit of address
      3 : NACK on transmit of data
      4 : Other error 
    */
    touchData.gesture = -1;
    touchData.x = 69;
    touchData.y = 69;
    return;  //If we get an unsuccessful probe, the device isn't awake, so just return
  }
  Wire.requestFrom(0x15, readBufSize);
  for (int x = 0; x < readBufSize; x++) {
    readBuf[x] = Wire.read();  //Read the data about the last touch event into the raw data buffer
  }
  /* 
    Byte 0 = gesture
    Byte 3 = x LSByte
    Byte 5  = y LSByte
  */
  touchData.gesture = readBuf[0];
  /* 
    Because the display is 240*240, the highest 4 bits of the positions are not used, meaning we can just use the lower byte of the position to get all the information we need
   */
  touchData.x = (readBuf[3]);  // << 8 | (uint16_t)readBuf[4]; (Not needed)
  touchData.y = (readBuf[5]);  // << 8 | (uint16_t)readBuf[6]; (Not needed)
}

/* 
  Get a pointer to the touchData struct containing the information about the last touch event
 */
touchDataStruct* getTouchDataStruct() {
  return &touchData;
}

/* 
  Put the touch panel to sleep
 */
void sleepTouchController() {
  Wire.beginTransmission(0x15);
  Wire.write(0xA5);
  Wire.write(0x03);
  Wire.endTransmission();
}