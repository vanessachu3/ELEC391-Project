#include <PWM.h>
#include <pid.h>
#include <Arduino.h>

#define MAXPWM 255
#define BAUD 115200
//PID_t pid;

#define SPEED_MODIFIER 0.1;
#define TURN_VAL 201

void setMotorPWM(float leftFwd, float leftBkwd, float rightFwd, float rightBkwd) {
    analogWrite(LEFTWHEEL_FWRD,   leftFwd);
    analogWrite(LEFTWHEEL_BKWRD,  leftBkwd);
    analogWrite(RIGHTWHEEL_FWRD,  rightFwd);
    analogWrite(RIGHTWHEEL_BKWRD, rightBkwd);
}
void moveRobot(const char* direction, float inputSignal, bool dlf, bool drt){
    inputSignal = (inputSignal >= 255) ? 255.0 : inputSignal; // Restrict inputSignal to 1.0 max
    float complementSignal = 255.0 - inputSignal;

    if (strcmp(direction, "BACKWARDS") == 0) {
        if(drt)
        {
            setMotorPWM(TURN_VAL,255,255,TURN_VAL);
        }
        else if(dlf)
        {
            setMotorPWM(255,TURN_VAL,TURN_VAL,255);
        }
        else
            setMotorPWM(complementSignal, 255, complementSignal, 255); //slow decay
    } 
    else if (strcmp(direction, "FORWARD") == 0) {
        if(drt)
        {
            setMotorPWM(TURN_VAL,255,255,TURN_VAL);
        }
        else if(dlf)
        {
            setMotorPWM(255,TURN_VAL,TURN_VAL,255);
        }
        else
        {
            setMotorPWM(255, complementSignal, 255, complementSignal); //slow
        }
    } 
}
