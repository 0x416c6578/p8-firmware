#include "headers/screenController.h"

/*
  Similar to ATCWatch, an instance of every screen will be instantiated at bootup
  There will be a pointer to the current screen which will have the methods called on it
  Finally a screen will be switched by moving the pointer to a different instance of a different screen
*/
TimeScreen timeScreen;
WatchScreenBase *currentScreen = &timeScreen;

/*
  This is called at bootup, so could be used to display a boot screen in the future
*/
void initScreen() {
  currentScreen->screenSetup();  //Call screenSetup() on the current screen (timeScreen) which at the moment does nothing
}

/* 
  This method is called AFAP by the main loop() in p8-firmware.ino
  The loop method of a screen should be as efficient as possible
    For example if any graphics are used, they should be drawn in setup rather than being redrawn every loop
 */
void screenControllerLoop() {
  currentScreen->screenLoop();
}