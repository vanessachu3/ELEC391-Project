#include <pid.h>
PID_t pid;
void updatePID(PID_t * pid, double angle)
{
    //update error/outputs:
    pid->e2 = pid->e1;
    pid->e1 = pid->e0;
    pid->u2 = pid->u1;
    pid->u1 = pid->u0;

    pid->angleRead = angle;
    pid->e0 =  pid->desiredAngle - pid->angleRead;
    pid->u0 = 1/pid->a0 * (-pid->a1*pid->u1 - pid->a2*pid->u2 + pid->b0*pid->e0 + pid->b1*pid->e1 + pid->b2*pid->e2);
    if (pid->u0 > 1)
    {pid->u0 = 1;}
    else if (pid->u0 <-1)
    {pid->u0 = -1;}
}

float getOutputPID(PID_t *pid)
{
    
    return pid->u0;
}

