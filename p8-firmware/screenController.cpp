#include "headers/screenController.h"

#define NUM_SCREENS 3
/*
  Similar to ATCWatch, an instance of every screen will be instantiated at bootup
  There will be a pointer to the current screen which will have the methods called on it
  Finally a screen will be switched by moving the pointer to a different instance of a different screen
*/
DemoScreen demoScreen;
TimeScreen timeScreen;
StopWatchScreen stopWatchScreen;

int currentHomeScreenIndex = 0;
WatchScreenBase* homeScreens[NUM_SCREENS] = {&timeScreen, &demoScreen, &stopWatchScreen};
WatchScreenBase* currentScreen = homeScreens[currentHomeScreenIndex];

/*
  This is called whenever a new screen is loaded
*/
void initScreen() {
  currentScreen->screenSetup();  //Call screenSetup() on the current screen
}

void handleLeftSwipe(){
  if (currentScreen->doesImplementSwipeLeft() == false){
    if (currentHomeScreenIndex == 0){
      currentHomeScreenIndex = NUM_SCREENS - 1;
      currentScreen = homeScreens[currentHomeScreenIndex];
      initScreen();
    } else {
      currentHomeScreenIndex--;
      currentScreen = homeScreens[currentHomeScreenIndex];
      initScreen();
    }
  }
  else
    currentScreen->swipeLeft();
}

void handleRightSwipe(){
  if (currentScreen->doesImplementSwipeRight() == false){
    if (currentHomeScreenIndex == NUM_SCREENS - 1){
      currentHomeScreenIndex = 0;
      currentScreen = homeScreens[currentHomeScreenIndex];
      initScreen();
    } else {
      currentHomeScreenIndex++;
      currentScreen = homeScreens[currentHomeScreenIndex];
      initScreen();
    }
  }
  else
    currentScreen->swipeRight();
}

/* 
  A button press will ALWAYS return to the time screen NO MATTER WHAT (no application 
  can have access to a button press event)
 */
void handleButtonPress(){
  if (currentHomeScreenIndex != 0){
    currentHomeScreenIndex = 0;
    currentScreen = homeScreens[currentHomeScreenIndex];
    initScreen();
  }
}


/* 
  Function called when a tap event is received by the interrupt handler 
  The parameters are the x and y coords of the tap
*/
void handleTap(uint8_t x, uint8_t y){
  currentScreen->screenTap(x, y);
}

/* 
  This method is called AFAP by the main loop() in p8-firmware.ino
  The loop method of a screen should be as efficient as possible
    For example if any graphics are used, they should be drawn in setup rather than being redrawn every loop
 */
void screenControllerLoop() {
  currentScreen->screenLoop();
}