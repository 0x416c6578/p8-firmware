#pragma once
#include <BLEPeripheral.h>

#include "Arduino.h"
#include "ioControl.h"

void initBluetooth();
void feedBle();
void connectHandler(BLECentral& central);
void disconnectHandler(BLECentral& central);
void writeHandler(BLECentral& central, BLECharacteristic& characteristic);