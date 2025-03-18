/**********************************************************************
PID contains all the required functions and data for PID control.
**********************************************************************/

#include "PID.h"
#include "PWM.h"
#include "Arduino_BMI270_BMM150.h"

#define max_speed 50.0f

PID_controller pid;

void PID_update(PID_controller *pid, float currAngle, float currMillis)
{
    
    pid->T = currMillis;

    // Serial.print(currAngle);
    // Serial.print("\t");

    // error signal
    float error = pid->setpoint - currAngle;

    // proportional
    float proportional = pid->kp * error;

    // Serial.print(proportional);
    // Serial.print("\t");

    // integral
    pid->integrator = pid->integrator + 0.5f
                    * pid->ki * pid->T * (error + pid->prev_error);

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
        pid->integrator = limMaxInt/8;

    else if(pid->integrator < limMinInt)
        pid->integrator = limMinInt/8;

    // Serial.print(pid->integrator);
    // Serial.print("\t");

    // differential
    pid->differentiator = (2.0f * pid->kd * (currAngle - pid->prevAngle)
                        + (2.0f * pid->tau - pid->T) * pid->differentiator)
                        / (2.0f * pid->tau + pid->T);

    // Serial.print(pid->differentiator);
    // Serial.print("\t");

    pid->output = proportional + pid->integrator + pid->differentiator;
    
    if(pid->output > pid->limMax)
        pid->output = pid->limMax;

    else if(pid->output < pid->limMin)
        pid->output = pid->limMin;

    pid->prev_error = error;
    pid->prevAngle = currAngle;

    if(pid->output < 0)
        PWM_bw(-pid->output); // mapped from [0,1] to [50,100]
    // 50.0f+50.0f*(-pid->output)
    else if(pid->output > 0)
        PWM_fw(pid->output);
    // 50.0f+50.0f*pid->output
    else if(pid->output == 0)
        PWM_stop();
    if(currAngle > 30 || currAngle < -30)
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