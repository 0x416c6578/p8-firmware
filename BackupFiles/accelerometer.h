#pragma once
#include <Wire.h>

#include "Arduino.h"
#include "bma423.h"
#include "pinoutP8.h"
#include "watchdog.h"
#include "i2c.h"
#include "ioControl.h"

void initAccel();
int8_t acclI2CRead(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
int8_t acclI2CWrite(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
void acclDelay(uint32_t period_us, void *intf_ptr);
bma4_accel* getAcclData();