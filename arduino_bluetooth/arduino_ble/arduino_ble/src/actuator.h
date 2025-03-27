#ifndef ACTUATOR_H
#define ACTUATOR_H
#include <Arduino.h>
#include <Servo.h>

#define BAUD 115200

#define servoLeft A7
#define servoRight A6

void actuatorSetup();
void actuatorLoop(const char* command);    
#endif