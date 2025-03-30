#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Arduino_BMI270_BMM150.h"


#define MAXPWM 255
#define BAUD 115200
#define PCT25 0.25
#define PCT50 0.50
#define PCT75 0.75
#define WAITTIME 10
#define LEFTWHEEL_FWRD    A0
#define LEFTWHEEL_BKWRD   A1
#define RIGHTWHEEL_FWRD   A2
#define RIGHTWHEEL_BKWRD  A3

void PWMfwrd(float scaleFactor);
void PWMbkwrd(float scaleFactor);
void task1();
void task2();
unsigned long secToMs(unsigned long sec);

void setup() {

  pinMode(LEFTWHEEL_FWRD, OUTPUT); 
  pinMode(LEFTWHEEL_BKWRD,  OUTPUT);
                                                                                                                                            
  pinMode(RIGHTWHEEL_FWRD,  OUTPUT);
  pinMode(RIGHTWHEEL_BKWRD,  OUTPUT);
  
}

void loop() {
  // PWM Outputs
  //PWMOutput();

  task1();
  task2();

}
unsigned long secToMs(unsigned long sec)
{
  return 1000*sec;
}

void task1() {
  // TASK 1 - Drive both motors same dirn/time (25%,50%,75%,100%)

  //////////////
  // FORWARD ///
  //////////////
  //Left motor - 25%
  analogWrite(LEFTWHEEL_FWRD,  MAXPWM*PCT25);
  analogWrite(LEFTWHEEL_BKWRD,   0);
  //Right Motor
  analogWrite(RIGHTWHEEL_FWRD,   MAXPWM*PCT25);
  analogWrite(RIGHTWHEEL_BKWRD,   0);
  delay(secToMs(WAITTIME));
  //Left motor
  analogWrite(LEFTWHEEL_FWRD,  MAXPWM*PCT50);
  analogWrite(LEFTWHEEL_BKWRD,   0);
  //Right Motor
  analogWrite(RIGHTWHEEL_FWRD,   MAXPWM*PCT50);
  analogWrite(RIGHTWHEEL_BKWRD,   0);
  delay(secToMs(WAITTIME));
  //Left motor
  analogWrite(LEFTWHEEL_FWRD,  MAXPWM*PCT75);
  analogWrite(LEFTWHEEL_BKWRD,   0);
  //Right Motor
  analogWrite(RIGHTWHEEL_FWRD,   MAXPWM*PCT75);
  analogWrite(RIGHTWHEEL_BKWRD,   0);
  delay(secToMs(WAITTIME));
  //Left motor
  analogWrite(LEFTWHEEL_FWRD,  MAXPWM);
  analogWrite(LEFTWHEEL_BKWRD,   0);
  //Right Motor
  analogWrite(RIGHTWHEEL_FWRD,   MAXPWM);
  analogWrite(RIGHTWHEEL_BKWRD,   0);
  delay(secToMs(WAITTIME));

  //////////////
  // BACKWARD //
  //////////////
  analogWrite(LEFTWHEEL_FWRD,  0);
  analogWrite(LEFTWHEEL_BKWRD,   MAXPWM*PCT25);
  //Right Motor
  analogWrite(RIGHTWHEEL_FWRD,   0);
  analogWrite(RIGHTWHEEL_BKWRD,   MAXPWM*PCT25);
  delay(secToMs(WAITTIME));
  //Left motor
  analogWrite(LEFTWHEEL_FWRD,  0);
  analogWrite(LEFTWHEEL_BKWRD,   MAXPWM*PCT75);
  //Right Motor
  analogWrite(RIGHTWHEEL_FWRD,   0);
  analogWrite(RIGHTWHEEL_BKWRD,   MAXPWM*PCT75);
  delay(secToMs(WAITTIME));

}

void task2() {
  // TASK 2 - 25%, 75% Both opposite dirn
  //Left motor - 25% - forward
  analogWrite(LEFTWHEEL_FWRD,  MAXPWM*PCT25);
  analogWrite(LEFTWHEEL_BKWRD,   0);
  //Right motor - 25% - backwards
  analogWrite(RIGHTWHEEL_FWRD,   0);
  analogWrite(RIGHTWHEEL_BKWRD,   MAXPWM*PCT25);
  delay(secToMs(WAITTIME));

  //Left motor - 25% - backwards
  analogWrite(LEFTWHEEL_FWRD,  0);
  analogWrite(LEFTWHEEL_BKWRD,   MAXPWM*PCT25);
  //Right motor - 25% - forwards
  analogWrite(RIGHTWHEEL_FWRD,   MAXPWM*PCT25);
  analogWrite(RIGHTWHEEL_BKWRD,   0);
  delay(secToMs(WAITTIME));

  //Left motor - 75%  - forward
  analogWrite(LEFTWHEEL_FWRD,  MAXPWM*PCT75);
  analogWrite(LEFTWHEEL_BKWRD,   0);
  //Right motor - 75% - backwards
  analogWrite(RIGHTWHEEL_FWRD,   0);
  analogWrite(RIGHTWHEEL_BKWRD,   MAXPWM*PCT75);
  delay(secToMs(WAITTIME));

  //Left motor - 75%  - forward
  analogWrite(LEFTWHEEL_FWRD,  0);
  analogWrite(LEFTWHEEL_BKWRD,   MAXPWM*PCT75);
  //Right motor - 75% - backwards
  analogWrite(RIGHTWHEEL_FWRD,   MAXPWM*PCT75);
  analogWrite(RIGHTWHEEL_BKWRD,   0);
  delay(secToMs(WAITTIME));
}