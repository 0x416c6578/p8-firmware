#include "pinoutP8.h"
#include "watchdog.h"
#include "ioControl.h"
#include "fastSPI.h"
#include "display.h"
#include "p8Time.h"
#include "nrf52.h"

void setup() {
  delay(500); //Coldboot cooldown time
  initIO(); //Init GPIOs
  if (getButtonState()){ //If the button is held at boot, go to the bootloader
    NRF_POWER->GPREGRET = 0x01;
    NVIC_SystemReset();
  }
  initWatchdog(); //Start the watchdog
  initFastSPI(); //Initialize EasyDMA SPI
  initDisplay(); //Initialize display
  randomTests();
}

void randomTests(){
  writeString(0,0,2,"Red", COLOUR_RED);
  writeString(0,20,2,"Green", COLOUR_GREEN);
  writeString(0,40,2,"Blue", COLOUR_BLUE);
  writeString(0,60,2,"White", COLOUR_WHITE);
  writeString(0,80,2,"Yellow", COLOUR_YELLOW);
  writeString(0,100,2,"Orange", COLOUR_ORANGE);
  writeString(0,120,2,"Cyan", COLOUR_CYAN);
  writeString(0,140,2,"Magenta", COLOUR_MAGENTA);
}

void loop(){
  if (!getButtonState()){
    feedWatchdog();
  }
}
