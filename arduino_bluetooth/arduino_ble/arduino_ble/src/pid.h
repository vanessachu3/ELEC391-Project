#ifndef PID_H
#define PID_H
typedef struct
{
    // tune these parameters
    float kp = 0.08; // 0.05;
    float ki = 0.1; // 0.005;
    float kd = 0.01; // 0.01;

    float N = 1 ; //per simulink

    float Ts = 1.0/99.86; //gyro sample rate

    float e2, e1, e0, u2, u1, u0  = 0;
    float angleRead;
    float desiredAngle = 0;

    //parameters:
    float b0 = kp*(1+N*Ts) + ki*Ts*(1+N*Ts) + kd*N;
    float b1 = -(kp*(2+N*Ts) + ki*Ts + 2*kd*N);
    float b2 = kp + N*kd;

    float a0 = 1+N*Ts;
    float a1 = -(2+Ts*N);
    float a2 = 1;

} PID_t;
extern PID_t pid;
void updatePID(PID_t * pid, double angle);
float getOutputPID(PID_t *pid);


#endif