#include "headers/powerControl.h"

uint8_t sleepTime = 10;
int lastWakeTime = 0;
bool powerMode = POWER_ON;

void initSleep() {
  sd_power_mode_set(NRF_POWER_MODE_LOWPWR);  //Use the softdevice wrapper to set the power mode when in CPU sleep
                                             //This is just a wrapper around inline assembly
                                             //svc 59
                                             //bx r14
  sd_power_dcdc_mode_set(NRF_POWER_DCDC_DISABLE);
}

/* 
  Enter sleep mode by disabling touch controller, display, backlight, led and motorOutput
 */
void enterSleep() {
  sleepTouchController();

  setPowerMode(POWER_OFF);
  displayEnable(POWER_OFF);
  setBrightness(BACKLIGHT_OFF);
  ledOutput(POWER_OFF);
  motorOutput(POWER_OFF);
}

/* 
  Exit sleep
 */
void exitSleep() {
  resetTouchController();

  setPowerMode(POWER_ON);
  displayEnable(POWER_ON);
  setBrightness(getBrightness());
  ledOutput(POWER_OFF);
  motorOutput(POWER_OFF);
}

/* 
  Set the global powerMode variable
 */
void setPowerMode(bool powerModeToSet) {
  powerMode = powerModeToSet;
}

/* 
  Get the current power state
 */
bool getPowerMode() {
  return powerMode;
}

/* 
  Whenever the device receives a touch or button interrupt, a global variable should
  be updated with the current millis().
  There should be a function in loop() that checks whether the current time is over
  n milliseconds of the current wake time, if so, it should put the device to sleep
 */
void updateLastWakeTime() {
  lastWakeTime = millis();
}

/* 
  If we have passed over the wake time, go to sleep
 */
void checkWakeTime() {
  if (millis() - lastWakeTime > sleepTime * 1000) {
    enterSleep();
  }
}

/* 
  Get the last wake time
 */
int getLastWakeTime(){
  return lastWakeTime;
}

/* 
  Set the time to sleep
 */
void setSleepTime(uint8_t seconds){
  sleepTime = seconds;
}

/* 
  Put the processor into lowest sleep state
 */
void sleepWait() {
  //Calling wait, send, wait fixes a bug where sometimes the CPU won't sleep the first time
  __WFE();
  __SEV();
  __WFE();
}