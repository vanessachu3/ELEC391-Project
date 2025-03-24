#ifndef PWM_H
#define PWM_H

#include <nRF52_MBED_PWM.hpp>

void PWM_init();
void PWM_fw(float pid_out);
void PWM_bw(float pid_out);
void PWM_lf(float pid_out);
void PWM_rt(float pid_out);
void PWM_stop();

#endif