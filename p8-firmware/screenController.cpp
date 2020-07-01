#include "headers/screenController.h"

#define NUM_SCREENS 4
/*
  Similar to ATCWatch, an instance of every screen will be instantiated at bootup
  There will be a pointer to the current screen which will have the methods called on it
  Finally a screen will be switched by moving the pointer to a different instance of a different screen
*/
//DemoScreen demoScreen;
TimeScreen timeScreen;
StopWatchScreen stopWatchScreen;
TimeDateSetScreen timeDateSetScreen;
DemoScreen demoScreen;

int currentHomeScreenIndex = 0;
WatchScreenBase* homeScreens[NUM_SCREENS] = {&timeScreen, &stopWatchScreen, &timeDateSetScreen, &demoScreen};
WatchScreenBase* currentScreen = homeScreens[currentHomeScreenIndex];

/*
  This is called whenever a new screen is loaded
*/
void initScreen() {
  currentScreen->screenSetup();  //Call screenSetup() on the current screen
  drawAppIndicator();
}

/* 
  Move to the right screen if the current screen doesn't have a handler for the right swipe,
  else call that handler 
*/
void handleLeftSwipe() {
  if (currentScreen->doesImplementSwipeRight() == false) {
    nextScreen();
  } else {
    currentScreen->swipeRight();
  }
}

/* 
  Move to the left screen if the current screen doesn't have a handler for the left swipe,
  else call that handler
 */
void handleRightSwipe() {
  if (currentScreen->doesImplementSwipeLeft() == false) {
    prevScreen();
  } else {
    currentScreen->swipeLeft();
  }
}

/* 
  Move to the left screen if you are not at screen 0
 */
void prevScreen() {
  if (currentHomeScreenIndex != 0) {
    currentHomeScreenIndex--;
    currentScreen = homeScreens[currentHomeScreenIndex];
    initScreen();
  }
}

/* 
  Move to the right screen if you aren't at the rightmost screen
 */
void nextScreen() {
  if (currentHomeScreenIndex != NUM_SCREENS - 1) {
    currentHomeScreenIndex++;
    currentScreen = homeScreens[currentHomeScreenIndex];
    initScreen();
  }
}

/*
  Since the main UI doesn't need a swipe up or down event (yet), just call the handler of the current screen
*/
void handleUpSwipe() {
  currentScreen->swipeUp();
}

/* 
  Ditto as above 
*/
void handleDownSwipe() {
  currentScreen->swipeDown();
}

/* 
  A button press will ALWAYS return to the time screen NO MATTER WHAT (no application 
  can have access to a button press event)
 */
void handleButtonPress() {
  if (currentHomeScreenIndex != 0) {
    currentHomeScreenIndex = 0;
    currentScreen = homeScreens[currentHomeScreenIndex];
    initScreen();
  }
}

/* 
  Function called when a tap event is received by the interrupt handler 
  The parameters are the x and y coords of the tap
*/
void handleTap(uint8_t x, uint8_t y) {
  if (y < 212) {
    currentScreen->screenTap(x, y);
  } else {
    if (x < 100) {
      prevScreen();
    } else if (x > 160) {
      nextScreen();
    }
  }
}

/* 
  This method is called AFAP by the main loop() in p8-firmware.ino
  The loop method of a screen should be as efficient as possible
    For example if any graphics are used, they should be drawn in setup rather than being redrawn every loop
 */
void screenControllerLoop() {
  currentScreen->screenLoop();
}

/*
Draw an indicator as to which screen you are currently on
*/
void drawAppIndicator() {
  drawRect(0, 213, 240, 1, COLOUR_WHITE);
  uint8_t indicatorFontSize = 3;
  uint8_t widthOfIndicator = getWidthOfNChars(NUM_SCREENS, 3);
  uint8_t startOfString = 120 - (widthOfIndicator / 2);
  //Draw the current screen indicators
  writeChar(startOfString + (0 * indicatorFontSize * FONT_WIDTH) + (0 * indicatorFontSize), 216, indicatorFontSize, (currentHomeScreenIndex == 0) ? GLYPH_CLOCK_SEL : GLYPH_CLOCK_UNSEL, COLOUR_WHITE, COLOUR_BLACK);
  writeChar(startOfString + (1 * indicatorFontSize * FONT_WIDTH) + (1 * indicatorFontSize), 216, indicatorFontSize, (currentHomeScreenIndex == 1) ? GLYPH_STOPWATCH_SEL : GLYPH_STOPWATCH_UNSEL, COLOUR_WHITE, COLOUR_BLACK);
  writeChar(startOfString + (2 * indicatorFontSize * FONT_WIDTH) + (2 * indicatorFontSize), 216, indicatorFontSize, (currentHomeScreenIndex == 2) ? GLYPH_SETTINGS_SEL : GLYPH_SETTINGS_UNSEL, COLOUR_WHITE, COLOUR_BLACK);
  writeChar(startOfString + (3 * indicatorFontSize * FONT_WIDTH) + (3 * indicatorFontSize), 216, indicatorFontSize, (currentHomeScreenIndex == 3) ? GLYPH_TOUCH_SEL : GLYPH_TOUCH_UNSEL, COLOUR_WHITE, COLOUR_BLACK);
  //Draw the "can scroll left/right" indicators in the corners of the screen
  switch (currentHomeScreenIndex) {
    case 0:
      writeChar(0, 216, indicatorFontSize, GLYPH_ARROW_LEFT, 0b1000010000010000, COLOUR_BLACK);
      writeChar(225, 216, indicatorFontSize, GLYPH_ARROW_RIGHT, COLOUR_WHITE, COLOUR_BLACK);
      break;
    case 1:
      writeChar(0, 216, indicatorFontSize, GLYPH_ARROW_LEFT, COLOUR_WHITE, COLOUR_BLACK);
      writeChar(225, 216, indicatorFontSize, GLYPH_ARROW_RIGHT, COLOUR_WHITE, COLOUR_BLACK);
      break;
    case 2:
      writeChar(0, 216, indicatorFontSize, GLYPH_ARROW_LEFT, COLOUR_WHITE, COLOUR_BLACK);
      writeChar(225, 216, indicatorFontSize, GLYPH_ARROW_RIGHT, COLOUR_WHITE, COLOUR_BLACK);
      break;
    case 3:
      writeChar(0, 216, indicatorFontSize, GLYPH_ARROW_LEFT, COLOUR_WHITE, COLOUR_BLACK);
      writeChar(225, 216, indicatorFontSize, GLYPH_ARROW_RIGHT, 0b1000010000010000, COLOUR_BLACK);
      break;
  }
}