#pragma once
#include <Wire.h>

#include "Arduino.h"
#include "bma423.h"
#include "headers/pinoutP8.h"
#include "headers/watchdog.h"

void initAccel();
int8_t busRead(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr);
int8_t busWrite(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr);
void delayFunction(uint32_t period_us, void *intf_ptr);