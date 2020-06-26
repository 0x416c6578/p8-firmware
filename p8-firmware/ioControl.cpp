#include "headers/ioControl.h"

int currentBrightness = 0;

/*
  Initialize various GPIOs, should be done at bootup
*/
void initIO() {
  pinMode(GREEN_LEDS, OUTPUT);
  pinMode(VIBRATOR_OUT, OUTPUT);
  pinMode(PUSH_BUTTON_IN, INPUT);
  pinMode(LCD_BACKLIGHT_LOW, OUTPUT);
  pinMode(LCD_BACKLIGHT_MID, OUTPUT);
  pinMode(LCD_BACKLIGHT_HIGH, OUTPUT);
  ledOutput(false);
  motorOutput(false);

  setBrightness(MAX_BRIGHTNESS);
}

/*
  Get state of push button
  Returns true if button is pressed
  Will probably not need to be used when interrupts are fully implemented
*/
bool getButtonState() {
  return digitalRead(PUSH_BUTTON_IN);
}

/*
  Turn the status LED on or off (LED on the heartrate sensor board)
*/
void ledOutput(bool on) {
  digitalWrite(GREEN_LEDS, on);
}

/*
  Turn the vibration motor on or off
*/
void motorOutput(bool on) {
  digitalWrite(VIBRATOR_OUT, !on);
}

/*
  Set the backlight brightness between 0b000 (low) and 0b111 or 7 (high)
  Since the backlight pins are active low, we must first invert the brightness
  Then we get the value of each bit by shifting it accordingly
  Finally we write that value to the output pin
  For example: brightness = 4 (100)
  Invert = 011
  Shifting yields: LOW = 1, MID = 1, HIGH = 0
*/
void setBrightness(int brightness) {
  if (brightness >= 0 && brightness <= 7) {  //Make sure the brightness is in the correct range
    currentBrightness = brightness;
    digitalWrite(LCD_BACKLIGHT_LOW, !(brightness & 1));
    digitalWrite(LCD_BACKLIGHT_MID, !((brightness >> 1) & 1));
    digitalWrite(LCD_BACKLIGHT_HIGH, !((brightness >> 2) & 1));
  }
}

/*
  Get current brightness
*/
int getBrightness() {
  return currentBrightness;
}

/*
  Increment brightness
*/
void incBrightness() {
  if (getBrightness() < 7) {
    setBrightness(getBrightness() + 1);
  }
}

/*
  Decrement brightness
*/
void decBrightness() {
  if (getBrightness() > 0) {
    setBrightness(getBrightness() - 1);
  }
}

/* 
Quickly flash green LEDs for debugging */
void ledPing(){
  digitalWrite(GREEN_LEDS, HIGH);
  delay(5);
  digitalWrite(GREEN_LEDS, LOW);
}