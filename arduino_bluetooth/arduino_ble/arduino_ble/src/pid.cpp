#include <pid.h>
#include <math.h>
PID_t pid;
void updatePID(PID_t *pid, double angle, float sampleSec) {
    // Update previous error and output values
    pid->e1 = pid->e0;
    pid->Ts = sampleSec;

    // Calculate current error
    pid->angleRead = angle;
    pid->e0 = pid->desiredAngle - pid->angleRead;

    // Proportional term
    pid->pTerm = pid->kp * pid->e0; //updated error

    // Integral term with anti-windup
    pid->iTemp += pid->e0 * pid->Ts;
    const float maxIntegral = 1.0f;
    if (pid->iTemp > maxIntegral) pid->iTemp = maxIntegral;
    else if (pid->iTemp < -maxIntegral) pid->iTemp = -maxIntegral;
    
    pid->iTerm = pid->ki * pid->iTemp;

    // Derivative term
    pid->dTemp = (pid->e0 - pid->e1) / pid->Ts;

    pid->dTerm = pid->kd * pid->dTemp;

    // Compute final output
    pid->u0 = pid->pTerm + pid->iTerm + pid->dTerm;

    // Safety shut-off for large angles
    if (fabs(angle) > 37.0) {
        pid->u0 = 0.0;
    }

    // Output saturation (limit to -1 to 1)
    if (pid->u0 > 1.0) pid->u0 = 1.0;
    else if (pid->u0 < -1.0) pid->u0 = -1.0;
}



float getOutputPID(PID_t *pid)
{
    
    return pid->u0;
}

