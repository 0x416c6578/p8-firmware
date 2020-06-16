#include "watchdog.h"
#include "pinoutP8.h"
//Interrupt controller
#include <nrf_nvic.h>
#include <nrf_sdm.h>
#include <nrf_soc.h>

/*
 * Initialize the watchdog for a 5 second timeout
 */
void initWatchdog(){
  enableWatchdog(5000);
}

/*
 * Enable the watchdog (code taken from atc1441)
 */
void enableWatchdog(int timeoutMillis){
  NRF_WDT->CONFIG = NRF_WDT->CONFIG = (WDT_CONFIG_HALT_Pause << WDT_CONFIG_HALT_Pos) | ( WDT_CONFIG_SLEEP_Pause << WDT_CONFIG_SLEEP_Pos);
  NRF_WDT->CRV = (32768 * timeoutMillis) / 1000;
  NRF_WDT->RREN |= WDT_RREN_RR0_Msk;
  NRF_WDT->TASKS_START = 1;
}

/*
 * Reset the watchdog timer
 */
void feedWatchdog(){
  NRF_WDT->RR[0] = WDT_RR_RR_Reload;
}
