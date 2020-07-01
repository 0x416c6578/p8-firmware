#include "headers/i2c.h"

#include <Wire.h>

//We have a mutex lock on the i2c object so we dont have any overlaps of accessing i2c
#define LOCKED true
#define UNLOCKED false
bool i2cLock = false;

/*
  Initialize the i2c bus to 250kbaud
*/
void initI2C() {
  Wire.begin();
  Wire.setClock(250000);
}

/* 
  Start an i2c transmission
 */
uint8_t i2cBeginTransmission(uint8_t id) {
  if (i2cLock == UNLOCKED) {
    Wire.beginTransmission(id);
    i2cLock = LOCKED;
    return 0;
  }
  return 1;
}

/* 
  End an i2c transmission, returning the result
 */
uint8_t i2cEndTransmission() {
  if (i2cLock == LOCKED) {
    i2cLock = UNLOCKED;
    return Wire.endTransmission();
  }
  return 255;
}

/* 
  Write a byte over i2c only when an transmission has been started
 */
void i2cWrite(uint8_t toWrite) {
  if (i2cLock == LOCKED) {
    Wire.write(toWrite);
  }
}

/* 
  Helper function to write a single byte
 */
void i2cWriteSingleByte(uint8_t id, uint8_t toWrite) {
  if (i2cLock == UNLOCKED) {
    i2cBeginTransmission(id);
    i2cWrite(toWrite);
    i2cEndTransmission();
  }
}

/*
  Read data from i2c bus into a byte buffer
*/
void i2cRead(uint8_t id, uint8_t* readBuffer, uint8_t readBufSize) {
  if (i2cLock == UNLOCKED) {
    Wire.requestFrom(id, readBufSize);
    for (int i = 0; i < readBufSize; i++) {
      readBuffer[i] = Wire.read();
    }
  }
}