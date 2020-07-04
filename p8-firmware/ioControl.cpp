#include "headers/ioControl.h"

int currentBrightness = 0;
uint16_t avgReading = 0;
long lastBatCalcTime = 0;
long lastBatPollTime = 0;
uint16_t batCounter = 0;
int cumulativeBatRead = 0;
uint16_t lastBatPercent = 69;

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
  //Disable flash
  pinMode(SPI_CE, OUTPUT);
  digitalWrite(SPI_CE, HIGH);

  //From battery.h
  pinMode(CHARGE_INDICATION, INPUT);
  pinMode(POWER_INDICATION, INPUT);
  pinMode(POWER_CONTROL, OUTPUT);
  digitalWrite(POWER_CONTROL, HIGH);

  setBrightness(3);
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
    if (brightness > 0)
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
void ledPing() {
  digitalWrite(GREEN_LEDS, HIGH);
  delay(5);
  digitalWrite(GREEN_LEDS, LOW);
}

/* 
  Only update the battery percent every 10 seconds to get a more accurate long-term reading
  every 10 seconds
 */
uint16_t getBatteryMV() {
  if (millis() - lastBatCalcTime > 6000) {
    lastBatCalcTime = millis();
    avgReading = cumulativeBatRead / batCounter;
    cumulativeBatRead = 0;
    batCounter = 0;
    lastBatPercent = milliVoltToPercent(avgReading);
    return lastBatPercent;
  } else {
    return lastBatPercent;
  }
}

/* 
  Every 500ms add to the cumulative battery reading 
*/
void addToCumulativeBatReading(){
  if (millis() - lastBatPollTime > 200) {
    batCounter++;
    cumulativeBatRead += map(analogRead(BATTERY_VOLTAGE), 496, 696, 3000, 4200);
    lastBatPollTime = millis();
  }
}

uint16_t milliVoltToPercent(int batteryMV) {
  if (batteryMV >= 4145)
    return 99;
  else if (batteryMV > 4010)
    return (99 - (int16_t)(4145 - batteryMV) / 13);
  else if (batteryMV > 3920)
    return (90 - (int16_t)(4010 - batteryMV) / 9);
  else if (batteryMV > 3850)
    return (80 - (int16_t)(3920 - batteryMV) / 7);
  else if (batteryMV > 3789)
    return (70 - (int16_t)(3850 - batteryMV) / 6);
  else if (batteryMV > 3740)
    return (60 - (int16_t)(3789 - batteryMV) / 4);
  else if (batteryMV > 3703)
    return (50 - (int16_t)(3740 - batteryMV) / 3);
  else if (batteryMV > 3676)
    return (40 - (int16_t)(3703 - batteryMV) / 2);
  else if (batteryMV > 3648)
    return (30 - (int16_t)(3676 - batteryMV) / 2);
  else if (batteryMV > 3609)
    return (20 - (int16_t)(3648 - batteryMV) / 3);
  else if (batteryMV > 3575)
    return (10 - (int16_t)(3609 - batteryMV) / 6);
  else if (batteryMV > 3482)
    return (5 - (int16_t)(3575 - batteryMV) / 18);
  else
    return 0;
}

/* 
  Get the battery charge state
 */
bool getChargeState(){
  return digitalRead(POWER_INDICATION);
}