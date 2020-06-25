#include "headers/interrupts.h"

bool ledStatus = false;

/*
  Function called when a touch event is detected (detects a falling edge on TP_INT)
*/
void touchEvent() {
  digitalWrite(GREEN_LEDS, HIGH);
  delay(5);
  digitalWrite(GREEN_LEDS, LOW);
}

/*
  At the moment I am using just the implementation of adding an interrupt from WInterrupts.h
  This implementation just uses the ARM/nRF register spec with little overhead
  In the future I would like to figure out how that works properly and make my own implementation
*/
void initInterrupts() {
  attachInterrupt(TP_INT, &touchEvent, FALLING);  //We want falling since the touch interrupt pin is active low
}
