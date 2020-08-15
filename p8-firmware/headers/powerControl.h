#pragma once

#include <nrf_nvic.h>

#include "Arduino.h"
#include "display.h"
#include "ioControl.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"
#include "nrf_soc.h"
#include "touch.h"
#include "utils.h"
#define POWER_ON 1
#define POWER_OFF 0
#define DEFAULT_SLEEP_TIME 10

void initSleep();
void enterSleep();
void exitSleep();
bool getPowerMode();
void sleepWait();
void setPowerMode(bool powerModeToSet);
void updateLastWakeTime();
void checkWakeTime();
int getLastWakeTime();
void setSleepTime(uint8_t seconds);