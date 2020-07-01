#include "headers/interrupts.h"

#define SLEEP_AFTER_N_SECONDS 10

uint16_t interruptsFlag = 0b0000000000000000;
long lastWakeTime = 0;
volatile bool lastPushButtonState;
volatile bool lastTouchState;

/*
  We say to the function handleInterrupts that there is a touch event to be handled
*/
void touchEvent() {
  updateTouchStruct();
  interruptsFlag |= TOUCH_INT;
}

/* 
  We have a similar case for the button event as for a touch event
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
  // attachInterrupt(TP_INT, &touchEvent, FALLING);  //We want falling since the touch interrupt pin is active low
  // attachInterrupt(PUSH_BUTTON_IN, &buttonEvent, RISING);
  NRF_GPIOTE->INTENCLR = GPIOTE_INTENSET_PORT_Msk;
  NVIC_DisableIRQ(GPIOTE_IRQn);
  NVIC_ClearPendingIRQ(GPIOTE_IRQn);
  NVIC_SetPriority(GPIOTE_IRQn, 1);
  NVIC_EnableIRQ(GPIOTE_IRQn);
  NRF_GPIOTE->EVENTS_PORT = 1;
  NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;

  lastPushButtonState = digitalRead(PUSH_BUTTON_IN);
  NRF_GPIO->PIN_CNF[PUSH_BUTTON_IN] |= ((uint32_t)(lastPushButtonState ? GPIO_PIN_CNF_SENSE_Low : GPIO_PIN_CNF_SENSE_High) << GPIO_PIN_CNF_SENSE_Pos);

  lastTouchState = digitalRead(TP_INT);
  NRF_GPIO->PIN_CNF[TP_INT] |= ((uint32_t)(lastTouchState ? GPIO_PIN_CNF_SENSE_Low : GPIO_PIN_CNF_SENSE_High) << GPIO_PIN_CNF_SENSE_Pos);
}

#ifdef __cplusplus
extern "C" {
#endif
void GPIOTE_IRQHandler() {
  if ((NRF_GPIOTE->EVENTS_PORT != 0)) {
    NRF_GPIOTE->EVENTS_PORT = 0;

    bool buttonRead = digitalRead(PUSH_BUTTON_IN);
    if (buttonRead != lastPushButtonState) {
      lastPushButtonState = buttonRead;
      NRF_GPIO->PIN_CNF[PUSH_BUTTON_IN] &= ~GPIO_PIN_CNF_SENSE_Msk;
      NRF_GPIO->PIN_CNF[PUSH_BUTTON_IN] |= ((lastPushButtonState ? GPIO_PIN_CNF_SENSE_Low : GPIO_PIN_CNF_SENSE_High) << GPIO_PIN_CNF_SENSE_Pos);
      if (lastPushButtonState == true)
        buttonEvent();
    }
    bool touchRead = digitalRead(TP_INT);
    if (touchRead != lastTouchState) {
      lastTouchState = touchRead;
      NRF_GPIO->PIN_CNF[TP_INT] &= ~GPIO_PIN_CNF_SENSE_Msk;
      NRF_GPIO->PIN_CNF[TP_INT] |= ((lastTouchState ? GPIO_PIN_CNF_SENSE_Low : GPIO_PIN_CNF_SENSE_High) << GPIO_PIN_CNF_SENSE_Pos);
      if (lastTouchState == false)
        touchEvent();
    }
  }
  (void)NRF_GPIOTE->EVENTS_PORT;
}
#ifdef __cplusplus
}
#endif

/* 
This method is called AFAP by the main Arduino loop()
Doing this means that crashes can't happen since the interrupt handling code is very efficient
Originally, having many function calls in the interrupt handling code (doing all the work during
the interrupt) could randomly cause a crash, this way doesn't
 */
void handleInterrupts() {
  if (interruptsFlag != 0) {
    if (getPowerMode() == POWER_OFF) {
      exitSleep();
      resetInterruptFlag();
      lastWakeTime = millis();
      return;
    }
    lastWakeTime = millis();

    switch (interruptsFlag) {
      case TOUCH_INT:
        handleTouchInt();
        resetInterruptFlag();
        break;
      case BUTTON_INT:
        handleButtonPress();  //Return to the time screen
        resetInterruptFlag();
        break;
    }
  } else {
    if (millis() - lastWakeTime > SLEEP_AFTER_N_SECONDS * 1000) {
      enterSleep();
    }
  }
}

/* 
  Originally this was called by the actuall interrupt handling function, but it caused problems
  when accessing the i2c bus when the thread code was also accessing it, randomly causing a crash
  So it was moved to a function that is called whilst in thread mode to stop the concurrent accessing
  of the i2c bus causing a crash
 */
void handleTouchInt() {
  switch (getTouchDataStruct()->gesture) {
    case SINGLE_TAP:
      handleTap(getTouchDataStruct()->x, getTouchDataStruct()->y);
      break;
    case LONG_PRESS:
      break;
    case SWIPE_DOWN:
      handleDownSwipe();
      break;
    case SWIPE_UP:
      handleUpSwipe();
      break;
    case SWIPE_LEFT:
      handleLeftSwipe();
      break;
    case SWIPE_RIGHT:
      handleRightSwipe();
      break;
  }
}

/* 
  Remove any flagged interrupts
 */
void resetInterruptFlag() {
  interruptsFlag = 0;
}