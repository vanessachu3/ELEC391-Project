#ifndef TIMERS_H
#define TIMERS_H

#include <PID.h>
#include <NRF52_MBED_TimerInterrupt.hpp>
#include <NRF52_MBED_ISR_Timer.hpp>

void TimerHandler();
void TIMERS_init();

void getOutput();

void IMU_init();
float getAngle();
// etc

#endif