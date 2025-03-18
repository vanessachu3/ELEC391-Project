/**********************************************************************
PWM will be responsible for managing the output PWM signals for each
motor. Will eventually need to include functions for driving. Also needs
some pins reserved for turn signal LEDs.
**********************************************************************/

#include "PWM.h"
#include <Arduino.h>

#define _PWM_LOGLEVEL_      0
#define FREQUENCY           1000.0f

// D10 & D9 for motorL, D8 & D7 for motorR ... subject to change
uint32_t pins[]         =   { D10, D9, D8, D7};

#define NUM_OF_PINS         ( sizeof(pins) / sizeof(uint32_t) )

float dutyCycle[]       =   { 0.0f, 0.0f, 0.0f, 0.0f };
float freq[]            =   { FREQUENCY, FREQUENCY, FREQUENCY, FREQUENCY };
float currCycle[]       =   { 0.0f, 0.0f, 0.0f, 0.0f };

mbed::PwmOut* pwm[]     =   { NULL, NULL, NULL, NULL };

void PWM_init()
{
    for(uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
        pinMode(pins[index], OUTPUT);
        digitalWrite(pins[index], LOW);
    }

    for(uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
        PWM_LOGERROR7("Freq = ", freq[index], ", \tDutyCycle % = ", dutyCycle[index], ", \tDutyCycle = ",
                      dutyCycle[index] / 100, ", \tPin = ", pins[index]);

        setPWM(pwm[index], pins[index], freq[index], dutyCycle[index]);
    }

    // turn green LED on to indicate PWM is active
    pinMode(LEDG, OUTPUT);
    digitalWrite(LEDG, LOW);    // turns LEDG on ..
}

void PWM_fw(float pid_out)
{
    for(uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
        if(index % 2 == 0)  // 0 or 2 --> D10 & D8
        {
            currCycle[index] = 0;
        }
        else
            currCycle[index] = pid_out;

        setPWM(pwm[index], pins[index], freq[index], currCycle[index]);
    }
}

void PWM_bw(float pid_out)
{
    for(uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
        if(index % 2 == 0)
        {
            currCycle[index] = pid_out;
        }
        else
            currCycle[index] = 0;

        setPWM(pwm[index], pins[index], freq[index], currCycle[index]);
    }
}

// void PWM_lf(volatile float pid_out)

// void PWM_rt(volatile float pid_out)

void PWM_stop()
{
    for(uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
        currCycle[index] = 0;
        setPWM(pwm[index], pins[index], freq[index], currCycle[index]);
    }
}