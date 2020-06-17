#include "pinoutP8.h"
#include "watchdog.h"
#include "ioControl.h"
#include "fastSPI.h"
#include "display.h"

void setup() {
  delay(500); //Coldboot cooldown time
  initIO(); //Init GPIOs
  if (getButton()){ //If the button is held at boot, go to the bootloader
    NRF_POWER->GPREGRET = 0x01;
    NVIC_SystemReset();
  }
  initWatchdog(); //Start the watchdog
  initFastSPI(); //Initialize EasyDMA SPI
  initDisplay(); //Initialize display
  writeString(0,0,1,"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
}

void loop() {
  if (!getButton()){
    feedWatchdog();
  }
  delay(4000);
}
