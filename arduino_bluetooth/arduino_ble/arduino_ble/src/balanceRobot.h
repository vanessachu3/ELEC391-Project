#ifndef BALANCEROBOT_H
#define BALANCEROBOT_H

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pid.h>
#include "Arduino_BMI270_BMM150.h"


#define BAUD 115200

float getAngleSetup();
float getAngle(float gyrSampleRate);
void PWMfwrd(float scaleFactor);
void PWMbkwrd(float scaleFactor);
void balance(PID_t * pid, float currAngle,float sampleSec);
void calibrateGyro(int samples = 500);
#endif