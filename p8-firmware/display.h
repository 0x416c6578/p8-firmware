#pragma once
#include "Arduino.h"
#include "colours.h"

void initDisplay();
void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t colour);
void setRowColRAMAddr(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void fillRectWithColour(uint16_t colour);
void clearDisplay();
void writeChar(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char character, uint16_t colourFG);
//Overloaded function for writing a string, thanks to c++ :)
void writeString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char* string, uint16_t colourFG = COLOUR_WHITE);
void writeString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, String string, uint16_t colourFG = COLOUR_WHITE);
void writeInt(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE);
void setDisplayPixels(int charColumn, int charRow, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colourFG);
