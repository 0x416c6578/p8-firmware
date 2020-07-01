#include "headers/interrupts.h"

#define SLEEP_AFTER_N_SECONDS 10

uint16_t interruptsFlag = 0b0000000000000000;
long lastWakeTime = 0;
bool pendingButtonInt = false;
bool pendingTouchInt = false;

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
  NRF_GPIO->PIN_CNF[PUSH_BUTTON_IN] |= ((uint32_t)(GPIO_PIN_CNF_SENSE_High) << GPIO_PIN_CNF_SENSE_Pos);
  NRF_GPIO->PIN_CNF[TP_INT] |= ((uint32_t)(GPIO_PIN_CNF_SENSE_Low) << GPIO_PIN_CNF_SENSE_Pos);
}

#ifdef __cplusplus
extern "C" {
#endif
void GPIOTE_IRQHandler() {
  if ((NRF_GPIOTE->EVENTS_PORT != 0)) {
    NRF_GPIOTE->EVENTS_PORT = 0;
    if (!digitalRead(TP_INT)) {
      updateTouchStruct();
      pendingTouchInt = true;
    } else if (digitalRead(PUSH_BUTTON_IN)) {
      pendingButtonInt = true;
    }
  }
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
  if (getPowerMode() == POWER_OFF) {
    exitSleep();
    resetInterrupts();
    lastWakeTime = millis();
    return;
  }

  if (pendingTouchInt) {
    TouchDataStruct* touchData = getTouchDataStruct();
    switch (touchData->gesture) {
      case SINGLE_TAP:
        handleTap(touchData->x, touchData->y);
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
  } else if (pendingButtonInt) {
    handleButtonPress();
  }

  else {
    if (millis() - lastWakeTime > SLEEP_AFTER_N_SECONDS * 1000) {
      enterSleep();
    }
  }

  resetInterrupts();
}

/* 
  Remove any flagged interrupts
 */
void resetInterrupts() {
  pendingTouchInt = false;
  pendingButtonInt = false;
}