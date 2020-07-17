#pragma once
#include "Arduino.h"
#include "colours.h"
#include "fastSPI.h"
#include "font.h"
#include "pinout.h"

void initDisplay();
void wakeDisplay();
void sleepDisplay();
void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t colour);
void setRowColRAMAddr(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void fillRectWithColour(uint16_t colour);
void clearDisplay(bool appDrawClear = false);
void writeChar(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char character, uint16_t colourFG, uint16_t colourBG);
void setDisplayPixels(int charColumn, int charRow, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colourFG, uint16_t colourBG);

//Overloaded function for writing a string, thanks to c++ :)
void writeString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char* string, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void writeString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, String string, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);

void writeIntWithoutPrecedingZeroes(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void writeIntWithPrecedingZeroes(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
uint32_t getWidthOfString(String string, uint8_t pixelsPerPixel);
uint32_t getWidthOfString(char* string, uint8_t pixelsPerPixel);
uint32_t getWidthOfNChars(uint8_t numChars, uint8_t pixelsPerPixel);
void drawRectOutline(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t colour);
void drawRectOutlineWithChar(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t rectColour, char character, uint8_t fontSize);