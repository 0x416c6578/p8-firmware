#pragma once
#include "pinoutP8.h"
#include "Arduino.h"

void initI2C();
uint8_t i2cBeginTransmission(uint8_t id);
uint8_t i2cEndTransmission();
void i2cWrite(uint8_t toWrite);
void i2cWriteSingleByte(uint8_t id, uint8_t toWrite);
void i2cRead(uint8_t id, uint8_t* readBuffer, uint8_t readBufSize);