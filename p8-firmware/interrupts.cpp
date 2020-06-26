#include "headers/interrupts.h"

/* 
  Here for reference:
  #define SINGLE_TAP_INT          0b0000000000000001
  #define LONG_PRESS_INT          0b0000000000000010
  #define SWIPE_UP_INT            0b0000000000000100
  #define SWIPE_DOWN_INT          0b0000000000001000
  #define SWIPE_LEFT_INT          0b0000000000010000
  #define SWIPE_RIGHT_INT         0b0000000000100000
  #define BUTTON_INT              0b0000000001000000
 */
uint16_t interruptsFlag = 0b0000000000000000;

/*
  Function called when a touch event is detected (detects a falling edge on TP_INT)
  We want this function to only set the interrupts flag, if we do a lot of work during 
  the interrupt handling routine, the device can crash
  Instead we want a function here that is called AFAP by the main Arduino loop() that will
  dispatch events / do other stuff depending on the interrupt flag
*/
void touchEvent() {
  updateTouchStruct();
  switch (getTouchDataStruct()->gesture){
    case SINGLE_TAP:
      interruptsFlag |= SINGLE_TAP_INT;
      break;
    case LONG_PRESS:
      interruptsFlag |= LONG_PRESS_INT;
      break;
    case SWIPE_DOWN:
      interruptsFlag |= SWIPE_DOWN_INT;
      break;
    case SWIPE_UP:
      interruptsFlag |= SWIPE_UP_INT;
      break;
    case SWIPE_LEFT:
      interruptsFlag |= SWIPE_LEFT_INT;
      break;
    case SWIPE_RIGHT:
      interruptsFlag |= SWIPE_RIGHT_INT;
      break;
  }
}

/* 
  We have a similar case for the button event as for a touch event, whereas this one is a lot simpler 
*/
void buttonEvent(){
  interruptsFlag |= BUTTON_INT;
}

/*
  At the moment I am using just the implementation of adding an interrupt from WInterrupts.h
  This implementation just uses the ARM/nRF register spec with little overhead
  In the future I would like to figure out how that works properly and make my own implementation
*/
void initInterrupts() {
  attachInterrupt(TP_INT, &touchEvent, FALLING);  //We want falling since the touch interrupt pin is active low
  attachInterrupt(PUSH_BUTTON_IN, &buttonEvent, RISING);
}

/* 
This method is called AFAP by the main Arduino loop()
Doing this means that crashes can't happen since the interrupt handling code is very efficient
Originally, having many function calls in the interrupt handling code (doing all the work during
the interrupt) could randomly cause a crash, this way doesn't
 */
void handleInterrupts(){
  if (interruptsFlag != 0){
    switch (interruptsFlag){
      case SINGLE_TAP_INT:
        handleTap(getTouchDataStruct()->x, getTouchDataStruct()->y);
        break;
      case LONG_PRESS_INT:
        break;
      case SWIPE_DOWN_INT:
        break;
      case SWIPE_UP_INT:
        break;
      case SWIPE_LEFT_INT:
        handleLeftSwipe();
        break;
      case SWIPE_RIGHT_INT:
        handleRightSwipe();
        break;
      case BUTTON_INT:
        handleButtonPress(); //Return to the time screen
        break;
    }
    resetInterruptFlag();
  }
}

/* 
  Remove any flagged interrupts
 */
void resetInterruptFlag(){
  interruptsFlag = 0;
}