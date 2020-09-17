#pragma once
#include "Arduino.h"
#include "colours.h"
#include "fastSPI.h"
#include "font.h"
#include "font16.h"
#include "pinout.h"
#include "utils.h"

//Old C style function definitions
void initDisplay();
void wakeDisplay();
void sleepDisplay();
void drawFilledRect(coord pos, uint32_t w, uint32_t h, uint16_t colour);
void setDisplayWriteRegion(coord pos, uint32_t w, uint32_t h);
void clearDisplay(bool leaveAppDrawer = false);
void drawChar(coord pos, uint8_t pixelsPerPixel, char character, uint16_t colourFG, uint16_t colourBG);
void drawCharPixelToBuffer(coord charPos, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colourFG, uint16_t colourBG);
void drawString(coord pos, uint8_t pixelsPerPixel, char* string, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void drawIntWithoutPrecedingZeroes(coord pos, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void drawIntWithPrecedingZeroes(coord pos, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void drawUnfilledRect(coord pos, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t colour);
void drawUnfilledRectWithChar(coord pos, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t rectColour, char character, uint8_t fontSize);
void writeNewChar(coord pos, char toWrite);