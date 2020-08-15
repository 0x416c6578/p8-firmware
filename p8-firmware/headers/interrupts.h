#pragma once
#include "Arduino.h"
#include "ioControl.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"
#include "pinout.h"
#include "powerControl.h"
#include "screenController.h"
#include "touch.h"
#include "utils.h"

/* 
  Interrupts are handled via the GPIOTE (GPIO Tasks and Events) driver: https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.nrf52832.ps.v1.1%2Fgpiote.html
  Implemented in WInterrupts.h
 */

void initInterrupts();
void handleInterrupts();
void resetInterrupts();