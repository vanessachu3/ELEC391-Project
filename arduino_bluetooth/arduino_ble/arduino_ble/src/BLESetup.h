#ifndef BLESETUP_H
#define BLESETUP_H

#include <Arduino.h>
#include <ArduinoBLE.h>
#include <PWM.h>

extern BLEService customService;
extern BLECharacteristic customCharacteristic;

void setupBLE();

#endif