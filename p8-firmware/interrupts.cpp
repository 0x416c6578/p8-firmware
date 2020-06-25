#include "headers/interrupts.h"
#include "headers/screenController.h"

bool ledStatus = false;

/* 
Function called when a touch event is detected (detects a falling edge on TP_INT)
 */
void touchEvent(){
  digitalWrite(GREEN_LEDS, HIGH);
  delay(5);
  digitalWrite(GREEN_LEDS, LOW);
}

void initInterrupts(){
  attachInterrupt(TP_INT, &touchEvent, FALLING); //We want falling since the touch interrupt pin is active low
}