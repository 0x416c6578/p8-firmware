#pragma once
#include "Arduino.h"

void initDisplay();
void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t colour);
void setRowColRAMAddr(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void displayColour(uint16_t colour);
void clearDisplay();
void writeChar(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char character, bool ignoreBlankPixels = false);
void writeString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, String string, bool ignoreBlankPixels = false);
