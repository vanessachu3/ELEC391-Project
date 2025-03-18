#ifndef PID_H
#define PID_H

typedef struct
{
    double kp        = 0.1;
    double ki        = 0;
    double kd        = 0;

    double T;                       // sample time (seconds)
    double tau       = 1*T;         // derivative low-pass filter time const.

    double limMin    = -100; 
    double limMax    = 100;         // output limits

    double integrator = 0;
    double prev_error = 0;
    double differentiator = 0;

    double prevAngle = 0;
    double output = 0;

    float setpoint  = 0;            // 0 degrees
} PID_controller;

// creates a global instance
extern PID_controller pid;

// void PID_init();
void PID_update(PID_controller *pid, float currAngle, float currMillis);

#endif