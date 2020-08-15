#pragma once
#include "Arduino.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"
#include "pinout.h"
#include "utils.h"

void initFastSPI();
void enableSPI(bool state);
void enableSingleByteWorkaround(NRF_SPIM_Type *spim, uint32_t ppi_channel, uint32_t gpiote_channel);
void disableSingleByteWorkaround(NRF_SPIM_Type *spim, uint32_t ppi_channel, uint32_t gpiote_channel);
void writeSPI(uint8_t *ptr, uint32_t len);
void writeSPISingleByte(uint8_t d);
void sendSPICommand(uint8_t command);
void preWrite();
void postWrite();
