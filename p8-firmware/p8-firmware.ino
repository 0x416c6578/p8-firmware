#include "headers/pinoutP8.h"
#include "headers/watchdog.h"
#include "headers/ioControl.h"
#include "headers/fastSPI.h"
#include "headers/display.h"
#include "headers/p8Time.h"
#include "headers/touch.h"
#include "nrf52.h"


void setup() {
  delay(500); //Coldboot cooldown time
  initIO(); //Init GPIOs
  if (getButtonState()) { //If the button is held at boot, go to the bootloader
    NRF_POWER->GPREGRET = 0x01;
    NVIC_SystemReset();
  }
  initWatchdog(); //Start the watchdog
  initFastSPI(); //Initialize EasyDMA SPI
  initDisplay(); //Initialize display
  initTouch(); //Initialize touch panel
  randomTests();
}

void randomTests() {

}

void loop() {
  if (!getButtonState()) feedWatchdog();
}
