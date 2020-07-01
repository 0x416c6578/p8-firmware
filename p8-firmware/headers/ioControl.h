#pragma once
#include "Arduino.h"
#include "pinoutP8.h"
#define MAX_BRIGHTNESS 7
#define MIN_BRIGHTNESS 1
#define BACKLIGHT_OFF 0

void initIO();
bool getButtonState();
void ledOutput(bool on);
void motorOutput(bool on);
void setBrightness(int brightness);
int getBrightness();
void incBrightness();
void decBrightness();
void ledPing();
uint16_t getBatteryMV();