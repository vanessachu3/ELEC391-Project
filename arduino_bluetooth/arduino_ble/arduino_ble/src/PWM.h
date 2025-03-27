#ifndef PWM_H
#define PWM_H

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pid.h>
#include "Arduino_BMI270_BMM150.h"

#define LEFTWHEEL_FWRD    D10
#define LEFTWHEEL_BKWRD   D9
#define RIGHTWHEEL_FWRD   D8
#define RIGHTWHEEL_BKWRD  D7


// Function prototypes for PWM control
void setMotorPWM(float leftFwd, float leftBkwd, float rightFwd, float rightBkwd);
void moveRobotCommand(const char* direction, float scaleFactor, PID_t *pid);
void moveRobot(const char* direction, float scaleFactor);

#endif