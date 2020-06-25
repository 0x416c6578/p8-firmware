#pragma once
#include <Wire.h>

#include "Arduino.h"
#include "pinoutP8.h"

#define NO_GESTURE 0x00
#define SLIDE_DOWN 0x01
#define SLIDE_UP 0x02
#define SLIDE_LEFT 0x03
#define SLIDE_RIGHT 0x04
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
} touchDataStruct;

void initTouch();
void resetTouchController(bool bootup = false);
void updateTouchStruct();
touchDataStruct* getTouchDataStruct();
void sleepTouchController();