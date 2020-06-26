#pragma once
#include <TimeLib.h>
#include "Arduino.h"

String getTimeWithSecs();
String getTime();
String getDate();
String getDay();
void setTimeWrapper(int yr, int mth, int day, int hr, int min, int sec);
String getStopWatchTime(int startTime, int currentTime);