#include "headers/bluetooth.h"

BLEPeripheral blePeripheral = BLEPeripheral();
BLEService bleService = BLEService("190A");

BLECharacteristic TXchar = BLECharacteristic("0002", BLENotify, 20);
BLECharacteristic RXchar = BLECharacteristic("0001", BLEWriteWithoutResponse, 20);

/* 
  Basically all code here was ripped from ATCWatch to enable the softdevice
  for power draw debugging. No work has been done on the bluetooth stack yet
 */
void initBluetooth() {
  blePeripheral.setLocalName("P8Watch1");
  blePeripheral.setAdvertisingInterval(500);
  blePeripheral.setDeviceName("P8Watch1");
  blePeripheral.setAdvertisedServiceUuid(bleService.uuid());
  blePeripheral.addAttribute(bleService);
  blePeripheral.addAttribute(TXchar);
  blePeripheral.addAttribute(RXchar);
  blePeripheral.setEventHandler(BLEConnected, connectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, disconnectHandler);
  RXchar.setEventHandler(BLEWritten, writeHandler);
  blePeripheral.begin();
  feedBle();
}

void feedBle() {
  blePeripheral.poll();
}

void connectHandler(BLECentral& central) {
  ledPing();
}

void disconnectHandler(BLECentral& central) {
  ledPing();
}

void writeHandler(BLECentral& central, BLECharacteristic& characteristic) {
  ledPing();
}