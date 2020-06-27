#include "headers/powerControl.h"

void initSleep() {
  sd_power_mode_set(NRF_POWER_MODE_LOWPWR);  //Use the softdevice wrapper to set the power mode when in CPU sleep
                                             //This is just a wrapper around inline assembly
                                             //svc 59
                                             //bx r14
}

bool powerMode = POWER_ON;

void enterSleep() {
  powerMode = POWER_OFF;
  displayEnable(POWER_OFF);
  setBrightness(BACKLIGHT_OFF);
  ledOutput(POWER_OFF);
  motorOutput(POWER_OFF);
}

void exitSleep() {
  powerMode = POWER_ON;
  displayEnable(POWER_ON);
  setBrightness(MIN_BRIGHTNESS);
  ledOutput(POWER_OFF);
  motorOutput(POWER_OFF);
}

bool getPowerMode(){
  return powerMode;
}

void sleepWait(){
  sd_app_evt_wait();
}