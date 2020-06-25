#include "headers/watchdog.h"

/*
  Initialize the watchdog for a 5 second timeout
*/
void initWatchdog() {
  enableWatchdog(5000);
}

/*
  Enable the watchdog (code taken from atc1441)
*/
void enableWatchdog(int timeoutMillis) {
  NRF_WDT->CONFIG = (WDT_CONFIG_HALT_Pause << WDT_CONFIG_HALT_Pos) | (WDT_CONFIG_SLEEP_Pause << WDT_CONFIG_SLEEP_Pos);
  NRF_WDT->CRV = (32768 * timeoutMillis) / 1000;  //Counter reload value
  NRF_WDT->RREN |= WDT_RREN_RR0_Msk;              //Enable reload request register 0 for resetting the watchdog
  NRF_WDT->TASKS_START = 1;                       //Start the watchdog
}

/*
  Reset the watchdog timer
*/
void feedWatchdog() {
  /*
    "To reload the watchdog counter, the special value 0x6E524635 needs to be written to all enabled reload registers"
    - From the documentation
  */
  NRF_WDT->RR[0] = WDT_RR_RR_Reload;  //Loads the watchdog reload value into register 0 (the reload register that was enabled in enableWatchdog())
}
