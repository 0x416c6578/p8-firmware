#include "headers/powerControl.h"

void initSleep() {
  sd_power_mode_set(NRF_POWER_MODE_LOWPWR);  //Use the softdevice wrapper to set the power mode when in CPU sleep
                                             //This is just a wrapper around inline assembly
                                             //svc 59
                                             //bx r14
  sd_power_dcdc_mode_set(NRF_POWER_DCDC_DISABLE);
}

bool powerMode = POWER_ON;

void enterSleep() {
  sleepTouchController();

  powerMode = POWER_OFF;
  displayEnable(POWER_OFF);
  setBrightness(BACKLIGHT_OFF);
  ledOutput(POWER_OFF);
  motorOutput(POWER_OFF);
}

void exitSleep() {
  resetTouchController();

  powerMode = POWER_ON;
  displayEnable(POWER_ON);
  setBrightness(getBrightness());
  ledOutput(POWER_OFF);
  motorOutput(POWER_OFF);
}

bool getPowerMode() {
  return powerMode;
}

void sleepWait() {
  __WFE();
  __SEV();
  __WFE();
}