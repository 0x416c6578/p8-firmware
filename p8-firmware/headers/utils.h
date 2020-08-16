#include "Arduino.h"
#pragma once

#define STR_WIDTH(str, size) ((sizeof(str) - 1) * size * FONT_WIDTH + (sizeof(str) - 2) * size)  //Macro to get the display width of a string literal
#define NCHAR_WIDTH(numChars, size) (numChars * size * FONT_WIDTH + (numChars - 1) * size)       //Macro to get the display width of n characters

/* 
  This structure is used for positions
 */
typedef struct {
  uint8_t x, y;
} coord;