#pragma once
#include <TimeLib.h>

#include "Arduino.h"
#include "utils.h"

void getTimeWithSecs(char* str);
void getTime(char* str);
void getDate(char* str);
void getDay(char* str);
void setTimeWrapper(int yr, int mth, int day, int hr, int min, int sec);
void getStopWatchTime(char* str, int startTime, int currentTime);
uint8_t getDayOfWeek(int d, int m, int y);
uint8_t getDayOfWeek();