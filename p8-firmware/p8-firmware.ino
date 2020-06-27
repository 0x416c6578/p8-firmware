#include "headers/display.h"
#include "headers/fastSPI.h"
#include "headers/interrupts.h"  //Includes screenController.h
#include "headers/ioControl.h"
#include "headers/p8Time.h"
#include "headers/pinoutP8.h"
#include "headers/powerControl.h"
#include "headers/touch.h"
#include "headers/watchdog.h"
#include "headers/heartrate.h"
#include "nrf52.h"

void setup() {
  initIO();                //Init GPIOs
  if (getButtonState()) {  //If the button is held at boot, enter bootloader
    NRF_POWER->GPREGRET = 0x01;
    NVIC_SystemReset();
  }
  initWatchdog();     //Start the watchdog
  initFastSPI();      //Initialize EasyDMA SPI
  initDisplay();      //Initialize display
  initTouch();        //Initialize touch panel
  initInterrupts();   //Setup interrupts
  initScreen();       //Initialize the screen controller (the thing that actually handles what is displayed)
  initSleep();        //Initialize the sleep power mode
  initHeartrate();    //Initialize the heartrate sensors
  //Disable ADC and PWM instances since they are not used (at the moment)
  NRF_SAADC->ENABLE = 0;
  NRF_PWM0->ENABLE = 0;
  NRF_PWM1->ENABLE = 0;
  NRF_PWM2->ENABLE = 0;
  NRF_RADIO->TASKS_DISABLE = 1; //Disable bluetooth radio at the moment
  randomTests();
}

void randomTests() {
  setTimeWrapper(2020, 6, 26, 15, 44, 0);
}

void loop() {
  if (!getButtonState()) feedWatchdog();
  if (getPowerMode() == POWER_ON){
    screenControllerLoop();  //This will run the main loop of the current screen
  }
  else{
    sleepWait();
  }
  handleInterrupts();  //If any interrupts were detected, this function will dispatch any events, and
                       //reset the interrupt flag
}
