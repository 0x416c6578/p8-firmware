#include "headers/i2cLock.h"

#include <Wire.h>

/* 
  This lock isn't really needed since the only thing that would affect it would be
  the touch interrupt handler, however since I fixed the bug with resetInterrupts,
  I moved the call to updateTouchData into thread code (rather than interrupt handling code)
  meaning that any i2c read/write is guaranteed to be atomic
  TODO: remove
 */

//We have a mutex lock on the i2c object so we dont have any overlaps of accessing i2c
bool i2cLock = false;

void initI2C() {
  Wire.begin();
  Wire.setClock(250000);
}

bool getI2CState(){
  return i2cLock;
}

void lockI2C(){
  i2cLock = I2C_LOCKED;
}

void unlockI2C(){
  i2cLock = I2C_UNLOCKED;
}