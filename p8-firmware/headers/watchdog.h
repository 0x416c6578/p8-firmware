#pragma once
#include "Arduino.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"
#include "pinoutP8.h"
//Interrupt controller stuff
#include <nrf_nvic.h>
#include <nrf_sdm.h>
#include <nrf_soc.h>

void initWatchdog();
void enableWatchdog(int timeoutMillis);
void feedWatchdog();
