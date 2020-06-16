#pragma once
#include "Arduino.h"

void initWatchdog();
void enableWatchdog(int timeoutMillis);
void feedWatchdog();
