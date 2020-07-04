#include "headers/display.h"
#include "headers/fastSPI.h"
//#include "headers/heartrate.h"
#include "headers/interrupts.h"  //Includes screenController.h
#include "headers/ioControl.h"
#include "headers/p8Time.h"
#include "headers/pinoutP8.h"
#include "headers/powerControl.h"
#include "headers/touch.h"
#include "headers/watchdog.h"
#include "headers/i2cLock.h"
#include "headers/accelerometer.h"
#include "nrf52.h"

void setup() {
  initIO();                //Init GPIOs
  if (getButtonState()) {  //If the button is held at boot, enter bootloader
    NRF_POWER->GPREGRET = 0x01;
    NVIC_SystemReset();
  }
  initWatchdog();    //Start the watchdog
  initFastSPI();     //Initialize EasyDMA SPI
  initDisplay();     //Initialize display
  initI2C();
  initAccel();
  initTouch();       //Initialize touch panel
  
  initInterrupts();  //Setup interrupts
  
  initScreen();      //Initialize the screen controller (the thing that actually handles what is displayed)
  initSleep();       //Initialize the sleep power mode
  //initHeartrate();   //Initialize the heartrate sensors
  randomTests();
}

void randomTests() {
}

void loop() {
  if (!getButtonState()) feedWatchdog();
  addToCumulativeBatReading();
  if (getPowerMode() == POWER_ON) {
    screenControllerLoop();  //This will run the main loop of the current screen
  } else {
    sleepWait();
  }
  handleInterrupts();  //If any interrupts were detected, this function will dispatch any events, and
                       //reset the interrupt flag
}
