#include <PWM.h>
#include <pid.h>

#define MAXPWM 255
#define BAUD 115200
//PID_t pid;

void setMotorPWM(float leftFwd, float leftBkwd, float rightFwd, float rightBkwd) {
    analogWrite(LEFTWHEEL_FWRD,   leftFwd);
    analogWrite(LEFTWHEEL_BKWRD,  leftBkwd);
    analogWrite(RIGHTWHEEL_FWRD,  rightFwd);
    analogWrite(RIGHTWHEEL_BKWRD, rightBkwd);
}
void moveRobot(const char* direction, float scaleFactor){
    scaleFactor = (scaleFactor >= 255) ? 255.0 : scaleFactor; // Restrict scaleFactor to 1.0 max
    float complementSignal = 255.0 - scaleFactor;

    if (strcmp(direction, "FORWARD") == 0) {
        setMotorPWM(255, complementSignal, 255, complementSignal); //slow decay
    } 
    else if (strcmp(direction, "BACKWARDS") == 0) {
        setMotorPWM(complementSignal, 255, complementSignal, 255); //slow
    } 
}
void moveRobotCommand(const char* direction, float scaleFactor, PID_t *pid) {
    scaleFactor = (scaleFactor >= 255) ? 255.0 : scaleFactor; // Restrict scaleFactor to 1.0 max
    float complementSignal = 255.0 - scaleFactor;

    if (strcmp(direction, "FORWARD") == 0) {
        //setMotorPWM(255, complementSignal, 255, complementSignal); //slow decay
        updateDesiredAngle(pid,-2.5);
    } 
    else if (strcmp(direction, "BACKWARDS") == 0) {
        //setMotorPWM(complementSignal, 255, complementSignal, 255); //slow
        updateDesiredAngle(pid,2.5);
    } 
    #if 0 
    else if (strcmp(direction, "LEFT") == 0) {
        setMotorPWM(0, complementSignal, complementSignal, 0);
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
    #endif

    // Serial.print("GOING ");
    // Serial.println(direction);
}
