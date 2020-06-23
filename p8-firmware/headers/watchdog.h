#pragma once
#include "Arduino.h"
#include "pinoutP8.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"

void initWatchdog();
void enableWatchdog(int timeoutMillis);
void feedWatchdog();
