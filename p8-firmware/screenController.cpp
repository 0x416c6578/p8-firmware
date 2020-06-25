#include "headers/screenController.h"

TimeScreen timeScreen;
WatchScreenBase *currentScreen = &timeScreen;

void initScreen(){
  currentScreen->screenSetup();
}

void screenControllerLoop(){
  currentScreen->screenLoop();
}