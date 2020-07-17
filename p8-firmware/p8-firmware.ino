#include <Wire.h>

#include "headers/accelerometer.h"
#include "headers/bluetooth.h"
#include "headers/display.h"
#include "headers/fastSPI.h"
#include "headers/interrupts.h"  //Includes screenController.h
#include "headers/ioControl.h"
#include "headers/p8Time.h"
#include "headers/pinout.h"
#include "headers/powerControl.h"
#include "headers/touch.h"
#include "headers/watchdog.h"
#include "nrf52.h"
void setup() {
  initIO();                //Init GPIOs
  if (getButtonState()) {  //If the button is held at boot, enter bootloader
    /* This sets a bit in the general purpose retention register which (I assume) 
    the bootloader sees and halts booting */
    NRF_POWER->GPREGRET = 0x01;
    NVIC_SystemReset();
  }
  initWatchdog();  //Start the watchdog
  initFastSPI();   //Initialize EasyDMA SPI
  initDisplay();   //Initialize display
  writeChar(100, 120 - 32, 8, GLYPH_SMILEY, COLOUR_WHITE, COLOUR_BLACK);
  Wire.begin();
  Wire.setClock(250000);
  initTouch();       //Initialize touch panel
  initAccel();       //Initialize the accelerometer
  initInterrupts();  //Setup interrupts
  initSleep();       //Initialize the sleep power mode
  initScreen();      //Setup the homescreen
  randomTests();     //Debugging stuff
}

void randomTests() {
  /* NRF_NVMC->CONFIG = 2;
  NRF_NVMC->ERASEUICR = 1;
  NVIC_SystemReset(); */
}

void loop() {
  if (!getButtonState()) {  //If the button is pressed, we don't want to feed the watchdog (allows for rebooting if needed)
    feedWatchdog();
  }

  //feedBle();

  addToCumulativeBatReading();  //Battery readings are taken over a period and the mean is taken periodically to get a more accurate reading

  if (getPowerMode() == POWER_ON) {
    screenControllerLoop();  //This will run the main loop of the current screen
  } else {
    sleepWait();  //This puts the MCU into its sleep mode, only waking on an interrupt
  }

  /* If any interrupts were detected, this function will dispatch any events, and
  reset the interrupt flag. This is not the NVIC interrupt handler, but a function called
  in the main loop. This means that the interrupt handlers can just be simple flag-sets, and we 
  can guarantee the atomic execution of interrupt handling (ie making sure that the interrupt handler
  doesn't read i2c whilst another part of the program was). */
  handleInterrupts();
}
