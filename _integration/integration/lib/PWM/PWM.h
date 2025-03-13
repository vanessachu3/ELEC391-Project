#ifndef PWM_H
#define PWM_H

#include <nRF52_MBED_PWM.hpp>

void PWM_init();
void PWM_fw(volatile float pid_out);
void PWM_bw(volatile float pid_out);
void PWM_lf(volatile float pid_out);
void PWM_rt(volatile float pid_out);

void PWM_stop();

#endif