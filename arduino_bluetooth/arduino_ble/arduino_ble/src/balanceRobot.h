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
float getAngle(PID_t * pid,float gyrSampleRate);
void PWMfwrd(float scaleFactor, bool dlf, bool drt);
void PWMbkwrd(float scaleFactor, bool dlf, bool drt);
void balance(PID_t * pid, float currAngle,float sampleSec, bool dfw, bool dbw, bool dlf, bool drt);
#endif