#ifndef PID_H
#define PID_H
typedef struct
{
    // tune these parameters
    float kp = 0.05; // 0.05;
    float ki = 0.03; // 0.005;
    float kd = 0.0001; // 0.01;
    float Ts = 1/99.8; //gyro sample rate
    float N = 1/Ts ; //per simulink 
    float e2, e1, e0, u2, u1, u0  = 0;
    float angleRead=0;
    float desiredAngle = 0;
    float pTerm = 0;
    float iTerm = 0;
    float dTerm = 0;
    float iTemp = 0;
    float dTemp = 0;
} PID_t;
extern PID_t pid;
void updatePID(PID_t * pid, double angle,float sampleSec);
float getOutputPID(PID_t *pid);


#endif