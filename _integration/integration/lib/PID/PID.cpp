/**********************************************************************
PID contains all the required functions and data for PID control.
**********************************************************************/

#include "PID.h"
#include "PWM.h"
#include "Arduino_BMI270_BMM150.h"

PID_controller pid;

void PID_update(PID_controller *pid, float currAngle, float currMillis)
{
    
    pid->T = currMillis;

    // error signal
    float error = pid->setpoint - currAngle;

    // proportional
    float proportional = error;

    // integral
    pid->integrator = pid->integrator + 0.5f
                    * pid->ki * currMillis * (error + pid->prev_error);

    // anti-wind up via integrator clamping
    float limMinInt, limMaxInt;

    // integral limits
    if(pid->limMax > proportional)
        limMaxInt = pid->limMax - proportional;

    else
        limMaxInt = 0.0f;

    if(pid->limMin < proportional)
        limMinInt = pid->limMin - proportional;

    else
        limMinInt = 0.0f;

    // clamp integrator
    if(pid->integrator > limMaxInt)
        pid->integrator = limMaxInt;

    else if(pid->integrator < limMinInt)
        pid->integrator = limMinInt;

    // differential
    pid->differentiator = (2.0f * (currAngle - pid->prevAngle)
                        + (2.0f * pid->tau - currMillis) * pid->differentiator)
                        / (2.0f * pid->tau + currMillis);

    pid->output = proportional*pid->kp + pid->integrator*pid->ki + pid->differentiator*pid->kd;
    
    if(pid->output > pid->limMax)
        pid->output = pid->limMax;

    else if(pid->output < pid->limMin)
        pid->output = pid->limMin;

    pid->prev_error = error;
    pid->prevAngle = currAngle;

    if(currAngle > 30 || currAngle < -30)
        PWM_stop();
    else if(pid->output < 0.0f)
        PWM_bw(25.0f+20.0f*(-pid->output)); // mapped from [0,1] to [50,100]
    // 50.0f+50.0f*(-pid->output)
    else if(pid->output > 0.0f)
        PWM_fw(25.0f+20.0f*pid->output);
    // 50.0f+50.0f*pid->output
    else if(pid->output == 0.0f)
        PWM_stop();
    

    Serial.print(pid->output);
    Serial.print("\t");
    Serial.print(proportional);
    Serial.print("\t");
    Serial.print(pid->integrator);
    Serial.print("\t");
    Serial.print(pid->differentiator);
    Serial.print("\t");
    Serial.println(currAngle);
};