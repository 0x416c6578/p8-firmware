#include "headers/interrupts.h"

#define SLEEP_AFTER_N_SECONDS 15

uint16_t interruptsFlag = 0b0000000000000000;
long lastWakeTime = 0;
bool pendingButtonInt = false;
bool pendingTouchInt = false;
bool lastButtonState;
bool lastTouchState;

/*
  Initialize interrupts using a GPIO port for reduced power draw
*/
void initInterrupts() {
  NRF_GPIOTE->INTENCLR = GPIOTE_INTENSET_PORT_Msk;
  NVIC_DisableIRQ(GPIOTE_IRQn);
  NVIC_ClearPendingIRQ(GPIOTE_IRQn);
  NVIC_SetPriority(GPIOTE_IRQn, 1);
  NVIC_EnableIRQ(GPIOTE_IRQn);

  //We use the port based interrupt to reduce power draw
  NRF_GPIOTE->EVENTS_PORT = 1;
  NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;

  lastButtonState = digitalRead(PUSH_BUTTON_IN);
  NRF_GPIO->PIN_CNF[PUSH_BUTTON_IN] |= (GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos);
  lastTouchState = digitalRead(TP_INT);
  NRF_GPIO->PIN_CNF[TP_INT] |= (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);

  // attachInterrupt(TP_INT, &touchEvent, FALLING);  //We want falling since the touch interrupt pin is active low
  // attachInterrupt(PUSH_BUTTON_IN, &buttonEvent, RISING);
}

/* 
  The interrupt handler works as follows:
  If the handler routine (below) is called, we know that there has been a change in state of any pin configured
  So we first check which pin was set that sent the interrupt for each pin configured to send an interrupt,
  then we change the interrupt configuration on that pin to only trigger when the inverse of the current read 
  value is seen. This means that an interrupt will only fire on a TRANSITION of state of any of the interrupt pins.
  This is effectively the same behaviour as GPIOTE LoToHi or HiToLo interrupts, however it has the added benefit 
  of drawing less power.
  Finally we only set the specific interrupt flag if the correct state of the interrupt is seen, for example
  only setting a button interrupt when the button is HIGH (since the button is active high) 
*/
#ifdef __cplusplus
extern "C" {
#endif
void GPIOTE_IRQHandler() {
  if ((NRF_GPIOTE->EVENTS_PORT != 0)) {
    NRF_GPIOTE->EVENTS_PORT = 0;  //Reset flag for the port event

    bool buttonRead = digitalRead(PUSH_BUTTON_IN);
    if (buttonRead != lastButtonState) {  //If there has been a change in state of the button
      lastButtonState = buttonRead;       //Set the last state accordingly
      NRF_GPIO->PIN_CNF[PUSH_BUTTON_IN] &= ~GPIO_PIN_CNF_SENSE_Msk;
      //Update the interrupt configuration to trigger on the inverse of the current read value (making sure interrupts are only triggered on state TRANSITIONS)
      NRF_GPIO->PIN_CNF[PUSH_BUTTON_IN] |= ((lastButtonState ? GPIO_PIN_CNF_SENSE_Low : GPIO_PIN_CNF_SENSE_High) << GPIO_PIN_CNF_SENSE_Pos);
      if (lastButtonState == HIGH) {
        pendingButtonInt = true;  //If we read a high button press (button is pressed), set the flag
      }
    }

    bool touchIntRead = digitalRead(TP_INT);
    if (touchIntRead != lastTouchState) {  //If there has been a change in state of the touch interrupt
      lastTouchState = touchIntRead;
      NRF_GPIO->PIN_CNF[TP_INT] &= ~GPIO_PIN_CNF_SENSE_Msk;
      NRF_GPIO->PIN_CNF[TP_INT] |= ((lastTouchState ? GPIO_PIN_CNF_SENSE_Low : GPIO_PIN_CNF_SENSE_High) << GPIO_PIN_CNF_SENSE_Pos);
      if (lastTouchState == LOW) {
        updateTouchStruct();
        pendingTouchInt = true;
      }
    }
  }
  (void)NRF_GPIOTE->EVENTS_PORT;
}
#ifdef __cplusplus
}
#endif

/* 
This method is called AFAP by the main Arduino loop()
 */
void handleInterrupts() {
  if (pendingTouchInt) {
    lastWakeTime = millis();
    TouchDataStruct *touchData = getTouchDataStruct();
    ledPing();
    switch (touchData->gesture) {
      case SINGLE_TAP:
        handleTap(touchData->x, touchData->y);
        break;
      case LONG_PRESS:
        handleLongTap(touchData->x, touchData->y);
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
    if (getPowerMode() == POWER_OFF) {
      exitSleep();
      resetInterrupts();
      lastWakeTime = millis();
      return;
    }
    if (millis() - lastWakeTime > 350) {
      lastWakeTime = millis();
      handleButtonPress();
    }
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