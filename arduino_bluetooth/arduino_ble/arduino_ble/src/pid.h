#ifndef PID_H
#define PID_H
#define DESIRED_ANGLE 0.2
typedef struct
{
    // tune these parameters
    float kp =9.0; //9;
    float ki =55.0; //65;
    float kd =0.2; //0.2;
    float Ts = 1.0/99.8; //gyro sample rate
    float e1 = 0.0;
    float e0 = 0.0; 
    float u0  = 0.0; //e[k-1], e[k], u[k]
    float angleRead=0.0;
    float desiredAngle = DESIRED_ANGLE;

    float pTerm = 0.0; // kp * e(t)
    float iTerm = 0.0; // ki *( iTerm[k-1] + Ts * e[k-1])
    float dTerm = 0.0; //kd * ( ef[k] - ef[k-1])/Ts
    float iTemp = 0.0;
    float dTemp = 0.0;
    float filterCoeff = 1;
} PID_t;
extern PID_t pid;
void updatePID(PID_t * pid, double angle,float sampleSec);
float getOutputPID(PID_t *pid);
void updateDesiredAngle(PID_t *pid, float angle);
void processSerialInput(PID_t *pid);
void updateDesiredAngleCommmand(PID_t *pid, const char* command);
float getDesiredAngle(PID_t*pid);
#endif