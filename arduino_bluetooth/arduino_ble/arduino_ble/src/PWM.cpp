#include <PWM.h>

#define MAXPWM 255
#define BAUD 115200
#define LEFTWHEEL_FWRD    A0
#define LEFTWHEEL_BKWRD   A1
#define RIGHTWHEEL_FWRD   A2
#define RIGHTWHEEL_BKWRD  A3


void setMotorPWM(float leftFwd, float leftBkwd, float rightFwd, float rightBkwd) {
    analogWrite(LEFTWHEEL_FWRD, MAXPWM * leftFwd);
    analogWrite(LEFTWHEEL_BKWRD, MAXPWM * leftBkwd);
    analogWrite(RIGHTWHEEL_FWRD, MAXPWM * rightFwd);
    analogWrite(RIGHTWHEEL_BKWRD, MAXPWM * rightBkwd);
}

void moveRobot(const char* direction, float scaleFactor) {
    scaleFactor = (scaleFactor >= 1) ? 1.0 : scaleFactor; // Clamp scaleFactor to 1.0 max

    if (strcmp(direction, "FORWARD") == 0) {
        setMotorPWM(scaleFactor, 0, scaleFactor, 0);
    } 
    else if (strcmp(direction, "BACKWARDS") == 0) {
        setMotorPWM(0, scaleFactor, 0, scaleFactor);
    } 
    else if (strcmp(direction, "LEFT") == 0) {
        setMotorPWM(0, scaleFactor, scaleFactor, 0);
    } 
    else if (strcmp(direction, "RIGHT") == 0) {
        setMotorPWM(scaleFactor, 0, 0, scaleFactor);
    } 
    else if (strcmp(direction, "FORWARD LEFT") == 0) {
        setMotorPWM(0.5 * scaleFactor, 0, scaleFactor, 0);
    } 
    else if (strcmp(direction, "FORWARD RIGHT") == 0) {
        setMotorPWM(scaleFactor, 0, 0.5 * scaleFactor, 0);
    } 
    else if (strcmp(direction, "BACKWARDS LEFT") == 0) {
        setMotorPWM(0, 0.5 * scaleFactor, 0, scaleFactor);
    } 
    else if (strcmp(direction, "BACKWARDS RIGHT") == 0) {
        setMotorPWM(0, scaleFactor, 0, 0.5 * scaleFactor);
    } 

    Serial.print("GOING ");
    Serial.println(direction);
}
