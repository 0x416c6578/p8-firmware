#include "headers/interrupts.h"

#define SLEEP_AFTER_N_SECONDS 10

uint16_t interruptsFlag = 0b0000000000000000;
int lastWakeTime = 0;


/*
  Function called when a touch event is detected (detects a falling edge on TP_INT)
  We want this function to only set the interrupts flag, if we do a lot of work during 
  the interrupt handling routine, the device can crash
  Instead we want a function here that is called AFAP by the main Arduino loop() that will
  dispatch events / do other stuff depending on the interrupt flag
*/
void touchEvent() {
  updateTouchStruct();
  switch (getTouchDataStruct()->gesture) {
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
void buttonEvent() {
  interruptsFlag |= BUTTON_INT;
}

/*
  Winterrupts.h uses GPIOTE Events for interrupt handling, which has the drawback of drawing more power
  Instead it is recommended to use a Port event to trigger the interrupt without increasing current draw
  That is what Aaron did in his implementation, however I am still yet to fully understand his implementation,
  so I am sticking with WInterrupts implementation
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
void handleInterrupts() {
  if (interruptsFlag != 0) {
    if (getPowerMode() == POWER_OFF)
      exitSleep();
    lastWakeTime = millis();
    switch (interruptsFlag) {
      case SINGLE_TAP_INT:
        handleTap(getTouchDataStruct()->x, getTouchDataStruct()->y);
        break;
      case LONG_PRESS_INT:
        break;
      case SWIPE_DOWN_INT:
        handleDownSwipe();
        break;
      case SWIPE_UP_INT:
        handleUpSwipe();
        break;
      case SWIPE_LEFT_INT:
        handleLeftSwipe();
        break;
      case SWIPE_RIGHT_INT:
        handleRightSwipe();
        break;
      case BUTTON_INT:
        handleButtonPress();  //Return to the time screen
        break;
    }
    resetInterruptFlag();
  } else{
    if (millis() - lastWakeTime > SLEEP_AFTER_N_SECONDS*1000){
      enterSleep();
    }
  }
}

/* 
  Remove any flagged interrupts
 */
void resetInterruptFlag() {
  interruptsFlag = 0;
}