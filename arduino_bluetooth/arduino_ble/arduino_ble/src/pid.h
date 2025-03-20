#ifndef PID_H
#define PID_H
typedef struct
{
    // tune these parameters
    float kp = 0.038927961;//0.038950; // 0.05;
    float ki = 0.000002740;//0.003000; // 0.005;
    float kd = 0.000002020;//0.000000030; // 0.01;
    float Ts = 1/99.8; //gyro sample rate
    float e1, e0, u0  = 0; //e[k-1], e[k], u[k]
    float angleRead=0;
    float desiredAngle = 0;

    float pTerm = 0; // kp * e(t)
    float iTerm = 0; // ki *( iTerm[k-1] + Ts * e[k-1])
    float dTerm = 0; //kd * ( ef[k] - ef[k-1])/Ts
    float iTemp = 0;
    float dTemp = 0;
    //float tau = 20*Ts;
    float filterCoeff = 0.4;
} PID_t;
extern PID_t pid;
void updatePID(PID_t * pid, double angle,float sampleSec);
float getOutputPID(PID_t *pid);

void processSerialInput(PID_t *pid);
#endif