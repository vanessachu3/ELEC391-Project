#ifndef PID_H
#define PID_H

typedef struct
{
    float kp                = 0.25;
    float ki                = 0.25;
    float kd                = 0;

    float T;                            // sample time (seconds)
    float tau               = 0.01*T;   // derivative low-pass filter time const.

    float limMin            = -1; 
    float limMax            = 1;        // output limits

    float integrator        = 0;
    float prev_error        = 0;
    float differentiator    = 0;

    float prevAngle         = 0;
    float output            = 0;

    float setpoint          = 0;        // 0 degrees
} PID_controller;

// creates a global instance
extern PID_controller pid;

// void PID_init();
void PID_update(PID_controller *pid, float currAngle, float currMillis);

#endif