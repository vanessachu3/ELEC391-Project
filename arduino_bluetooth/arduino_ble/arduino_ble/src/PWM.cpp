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
void moveRobot(const char* direction, float inputSignal){
    inputSignal = (inputSignal >= 255) ? 255.0 : inputSignal; // Restrict inputSignal to 1.0 max
    float complementSignal = 255.0 - inputSignal;

    if (strcmp(direction, "FORWARD") == 0) {
        setMotorPWM(255, complementSignal, 255, complementSignal); //slow decay
    } 
    else if (strcmp(direction, "BACKWARDS") == 0) {
        setMotorPWM(complementSignal, 255, complementSignal, 255); //slow
    } 
}
void moveRobotCommand(const char* direction, float inputSignal, PID_t *pid) {
    inputSignal = (inputSignal >= 255) ? 255.0 : inputSignal; // Restrict inputSignal to 1.0 max
    float complementSignal = 255.0 - inputSignal;

    if (strcmp(direction, "FORWARD") == 0) {
        //setMotorPWM(255, complementSignal, 255, complementSignal); //slow decay
        updateDesiredAngle(pid,-(3+abs(DESIRED_ANGLE)));
    } 
    else if (strcmp(direction, "BACKWARDS") == 0) {
        //setMotorPWM(complementSignal, 255, complementSignal, 255); //slow
        updateDesiredAngle(pid,3+abs(DESIRED_ANGLE));
    } 
    #if 1
    else if (strcmp(direction, "LEFT") == 0) {
        setMotorPWM(255, complementSignal*0.5, complementSignal, 255);
    } 
    else if (strcmp(direction, "RIGHT") == 0) {
        setMotorPWM(complementSignal, 255, 255, complementSignal*0.5);
    } 
    #endif
    #if 0
    else if (strcmp(direction, "FORWARD LEFT") == 0) {
        setMotorPWM(0.5 * inputSignal, 0, inputSignal, 0);
    } 
    else if (strcmp(direction, "FORWARD RIGHT") == 0) {
        setMotorPWM(inputSignal, 0, 0.5 * inputSignal, 0);
    } 
    else if (strcmp(direction, "BACKWARDS LEFT") == 0) {
        setMotorPWM(0, 0.5 * inputSignal, 0, inputSignal);
    } 
    else if (strcmp(direction, "BACKWARDS RIGHT") == 0) {
        setMotorPWM(0, inputSignal, 0, 0.5 * inputSignal);
    } 
    #endif

    // Serial.print("GOING ");
    // Serial.println(direction);
}
