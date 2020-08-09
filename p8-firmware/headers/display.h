#pragma once
#include "Arduino.h"
#include "colours.h"
#include "fastSPI.h"
#include "font.h"
#include "font16.h"
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
  static void drawCharPixelToBuffer(int charColumn, int charRow, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colourFG, uint16_t colourBG);
}; */

typedef struct {
  uint8_t x,y;
} point;

//Old C style function definitions
void initDisplay();
void wakeDisplay();
void sleepDisplay();
void drawFilledRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t colour);
void setDisplayWriteRegion(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void fillDisplayRam(uint16_t colour);
void clearDisplay(bool leaveAppDrawer = false);
void drawChar(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char character, uint16_t colourFG, uint16_t colourBG);
void drawCharPixelToBuffer(int charColumn, int charRow, uint8_t pixelsPerPixel, bool pixelInCharHere, uint16_t colourFG, uint16_t colourBG);
void drawString(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, char* string, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void drawIntWithoutPrecedingZeroes(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void drawIntWithPrecedingZeroes(uint32_t x, uint32_t y, uint8_t pixelsPerPixel, int toWrite, uint16_t colourFG = COLOUR_WHITE, uint16_t colourBG = COLOUR_BLACK);
void drawUnfilledRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t colour);
void drawUnfilledRectWithChar(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t lineWidth, uint16_t rectColour, char character, uint8_t fontSize);
void writeNewChar(uint8_t x, uint8_t y, char toWrite);