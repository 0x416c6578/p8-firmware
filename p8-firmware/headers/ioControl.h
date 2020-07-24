#pragma once
#include "Arduino.h"
#include "pinout.h"
#define MAX_BRIGHTNESS 7
#define MIN_BRIGHTNESS 1
#define BACKLIGHT_OFF 0

/* class IOController {
 private:
  static IOController* instance;
  IOController();
  uint8_t currentBrightness;


 public:
  static IOController* getInstance();
  void init();
  void setLED(bool state);
  void setMotor(bool state);
  void setBrightness(int brightness);
}; */

void initIO();
bool getButtonState();
void ledOutput(bool on);
void motorOutput(bool on);
void setBrightness(int brightness);
int getBrightness();
void incBrightness();
void decBrightness();
void ledPing();
uint16_t getBatteryPercent();
uint16_t milliVoltToPercent(int batteryMV);
void addToCumulativeBatReading();
bool getChargeState();