#pragma once

#include <nrf_nvic.h>
#include "nrf_soc.h"

#include "Arduino.h"
#include "display.h"
#include "ioControl.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"
#include "touch.h"
#define POWER_ON 1
#define POWER_OFF 0

void initSleep();
void enterSleep();
void exitSleep();
bool getPowerMode();
void sleepWait();