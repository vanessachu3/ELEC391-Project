#ifndef PWM_H
#define PWM_H

// Define constants (same as in PWM.cpp)
#define MAXPWM 255

// Function prototypes for PWM control
void PWMfwrd(float scaleFactor);
void PWMbkwrd(float scaleFactor);
void PWMleft(float scaleFactor);
void PWMright(float scaleFactor);
void PWMfwrdLeft(float scaleFactor);
void PWMfwrdRight(float scaleFactor);
void PWMbkwrdLeft(float scaleFactor);
void PWMbkwrdRight(float scaleFactor);

#endif