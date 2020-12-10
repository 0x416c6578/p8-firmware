# the screenController

After all the hardware and interrupts are initialized, screenController::initScreen() is called. We're finally getting into the details of applications instead of hardware.

There is an array of screens set up, and currentScreen points to an element of that array. At startup, the currentScreen is set to timeScreen.

currentScreen->screenSetup() (timeScreen->screenSetup()) does very little.
drawAppIndicator() draws the app bar
screenUpdateMS = currentScreen->getScreenUpdateTimeMS() sets the current screen update time

