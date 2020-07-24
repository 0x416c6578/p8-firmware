#pragma once
#include "Arduino.h"
#include "colours.h"
#include "fastSPI.h"
#include "font.h"
#include "pinout.h"

/* class Display {
 public:
  static void init();
  static void wake();
  static void sleep();
  static void drawFilledRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t colour);
  static void clear(bool appDrawClear = false);
  static void drawChar(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char character, uint16_t colourFG, uint16_t colourBG);
  static void drawString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char* string, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
  static void drawString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, String string, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
  static void drawInt(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK, bool precedingZeroes = false);
  static void drawUnfilledRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t colour);
  static void drawUnfilledRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t rectColour, char character, uint8_t fontSize);

 private:
  static void setWriteWindow(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
  static void writeRectToDisplay(uint16_t colour);
  static void writeCharPixelToBuffer(int charColumn, int charRow, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colourFG, uint16_t colourBG);
}; */

//Old C style function definitions
void initDisplay();
void wakeDisplay();
void sleepDisplay();
void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t colour);
void setRowColRAMAddr(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void fillRectWithColour(uint16_t colour);
void clearDisplay(bool appDrawClear = false);
void writeChar(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char character, uint16_t colourFG, uint16_t colourBG);
void setDisplayPixels(int charColumn, int charRow, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colourFG, uint16_t colourBG);
void writeString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char* string, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void writeIntWithoutPrecedingZeroes(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void writeIntWithPrecedingZeroes(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
uint32_t getWidthOfNChars(uint8_t numChars, uint8_t pixelsPerPixel);
void drawRectOutline(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t colour);
void drawRectOutlineWithChar(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t rectColour, char character, uint8_t fontSize);