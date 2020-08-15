#pragma once
#include <nrf_nvic.h>
#include <nrf_sdm.h>
#include <nrf_soc.h>

#include "Arduino.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"
#include "pinout.h"
#include "utils.h"

void initWatchdog();
void enableWatchdog(int timeoutMillis);
void feedWatchdog();
