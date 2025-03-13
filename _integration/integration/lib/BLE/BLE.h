#ifndef BLE_H
#define BLE_H

#include <ArduinoBLE.h>

extern BLEService customService;
extern BLECharacteristic customCharacteristic;

void BLE_init();

#endif