#include "headers/bluetooth.h"

BLEPeripheral blePeripheral = BLEPeripheral();
BLEService bleService = BLEService("190A");

void initBluetooth() {
  blePeripheral.setLocalName("P8");
  blePeripheral.setAdvertisingInterval(500);
  blePeripheral.setDeviceName("P8");
  blePeripheral.setAdvertisedServiceUuid(bleService.uuid());
  blePeripheral.addAttribute(bleService);
  blePeripheral.setEventHandler(BLEConnected, connectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, disconnectHandler);
  blePeripheral.begin();
  feedBle();
}

void feedBle(){
  blePeripheral.poll();
}

void connectHandler(BLECentral& central){
  ledPing();
}

void disconnectHandler(BLECentral& central){
  ledPing();
}