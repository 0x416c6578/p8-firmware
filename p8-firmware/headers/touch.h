#pragma once
#include <Wire.h>

#include "Arduino.h"
#include "ioControl.h"
#include "pinout.h"

#define NO_GESTURE 0x00
#define SWIPE_DOWN 0x01
#define SWIPE_UP 0x02
#define SWIPE_LEFT 0x03
#define SWIPE_RIGHT 0x04
#define SINGLE_TAP 0x05
#define DOUBLE_TAP 0x0B
#define LONG_PRESS 0x0C

/* 
  This struct will hold the data collected from the touch controller
  x = touch x position
  y = touch y position
  gesture = byte representing the gesture (see definitions)
 */
typedef struct {
  uint8_t gesture;
  uint8_t x;
  uint8_t y;
} TouchDataStruct;

void initTouch();
void resetTouchController(bool bootup = false);
void updateTouchStruct();
TouchDataStruct* getTouchDataStruct();
void sleepTouchController();