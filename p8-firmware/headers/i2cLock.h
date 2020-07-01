#pragma once
#include "pinoutP8.h"
#include "Arduino.h"

#define I2C_LOCKED true
#define I2C_UNLOCKED false

void initI2C();
bool getI2CState();
void lockI2C();
void unlockI2C();