#pragma once
#include <TimeLib.h>
#include "Arduino.h"
void printTimeStringWithSeconds(uint32_t x, uint32_t y, uint16_t colour);
void printTimeString(uint32_t x, uint32_t y, uint16_t colour);
void printDateString(uint32_t x, uint32_t y, uint16_t colour);
void printDayOfWeekString(uint32_t x, uint32_t y, uint16_t colour);
void setTimeWrapper(int yr, int mth, int day, int hr, int min, int sec);
