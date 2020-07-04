#pragma once
#include "Arduino.h"
#include "ioControl.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"
#include "pinoutP8.h"
#include "screenController.h"
#include "touch.h"
/* 
  #define NO_GESTURE 0x00
  #define SLIDE_DOWN 0x01
  #define SLIDE_UP 0x02
  #define SLIDE_LEFT 0x03
  #define SLIDE_RIGHT 0x04
  #define SINGLE_TAP 0x05
  #define DOUBLE_TAP 0x0B
  #define LONG_PRESS 0x0C 
*/

/* #define SINGLE_TAP_INT          0b0000000000000001
#define LONG_PRESS_INT          0b0000000000000010
#define SWIPE_UP_INT            0b0000000000000100
#define SWIPE_DOWN_INT          0b0000000000001000
#define SWIPE_LEFT_INT          0b0000000000010000
#define SWIPE_RIGHT_INT         0b0000000000100000 */
#define BUTTON_INT              0b0000000000000001
#define TOUCH_INT               0b0000000000000010

/* 
  Interrupts are handled via the GPIOTE (GPIO Tasks and Events) driver: https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.nrf52832.ps.v1.1%2Fgpiote.html
  Implemented in WInterrupts.h
 */

void initInterrupts();
void handleInterrupts();
void resetInterrupts();