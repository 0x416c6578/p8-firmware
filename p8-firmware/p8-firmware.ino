#include "headers/display.h"
#include "headers/fastSPI.h"
#include "headers/interrupts.h"  //Includes screenController.h
#include "headers/ioControl.h"
#include "headers/p8Time.h"
#include "headers/pinoutP8.h"
#include "headers/touch.h"
#include "headers/watchdog.h"
#include "nrf52.h"

void setup() {
  delay(500);              //Coldboot cooldown time
  initIO();                //Init GPIOs
  if (getButtonState()) {  //If the button is held at boot, enter bootloader
    NRF_POWER->GPREGRET = 0x01;
    NVIC_SystemReset();
  }
  initWatchdog();    //Start the watchdog
  initFastSPI();     //Initialize EasyDMA SPI
  initDisplay();     //Initialize display
  initTouch();       //Initialize touch panel
  initInterrupts();  //Setup interrupts
  initScreen();      //Initialize the screen controller (the thing that actually handles what is displayed)
  randomTests();
}

void randomTests() {
  setTimeWrapper(2020, 6, 26, 15, 44, 0);
}

void loop() {
  if (!getButtonState()) feedWatchdog();
  screenControllerLoop();  //This will run the main loop of the current screen
  handleInterrupts();      //If any interrupts were detected, this function will dispatch any events, and
                           //reset the interrupt flag
}
