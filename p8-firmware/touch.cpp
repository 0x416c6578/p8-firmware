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

TouchDataStruct touchData = {-1, -1, -1};

/* 
  Initialize the touch panel (basically just reset and put into running (not sleeping) state)
*/
void initTouch() {
  pinMode(TP_RESET, OUTPUT);  //Reset pin
  pinMode(TP_INT, INPUT);     //Interrupt pin
  resetTouchController(true);
  lockI2C();
  Wire.beginTransmission(0x15);
  Wire.write(0xED);
  Wire.write(0xC8);
  Wire.endTransmission();
  unlockI2C();
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
  if (getI2CState() == I2C_LOCKED){
    return;
  }
  lockI2C();

  uint8_t readBufSize = 6;
  uint8_t readBuf[readBufSize];  //Read buffer where the raw bytes are stored
  Wire.beginTransmission(0x15);
  Wire.write(0x1);  //Try to ping the touch controller
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
  for (uint8_t i = 0; i < readBufSize; i++){
    readBuf[i] = Wire.read();
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
  ledPing();

  unlockI2C();
}

/* 
  Get a pointer to the touchData struct containing the information about the last touch event
 */
TouchDataStruct* getTouchDataStruct() {
  return &touchData;
}

/* 
  Put the touch panel to sleep
 */
void sleepTouchController() {
  lockI2C();
  Wire.beginTransmission(0x15);
  Wire.write(0xA5);
  Wire.write(0x03);
  delay(20);
  Wire.endTransmission();
  unlockI2C();
}