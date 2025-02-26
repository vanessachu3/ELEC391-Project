#ifndef PWM_H
#define PWM_H

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Arduino_BMI270_BMM150.h"


// Function prototypes for PWM control
void setMotorPWM(float leftFwd, float leftBkwd, float rightFwd, float rightBkwd);
void moveRobot(const char* direction, float scaleFactor);

#endif