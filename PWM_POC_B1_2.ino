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


void PWMfwrd(float scaleFactor);
void PWMbkwrd(float scaleFactor);
void task1();
void task2();
unsigned long secToMs(unsigned long sec);

void setup() {

  pinMode(A0, OUTPUT); 
  pinMode(A1,  OUTPUT);
                                                                                                                                            
  pinMode(A2,  OUTPUT);
  pinMode(A3,  OUTPUT);
  
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
  analogWrite(A0,  MAXPWM*PCT25);
  analogWrite(A1,   0);
  //Right Motor
  analogWrite(A2,   MAXPWM*PCT25);
  analogWrite(A3,   0);
  delay(secToMs(WAITTIME));
  //Left motor
  analogWrite(A0,  MAXPWM*PCT50);
  analogWrite(A1,   0);
  //Right Motor
  analogWrite(A2,   MAXPWM*PCT50);
  analogWrite(A3,   0);
  delay(secToMs(WAITTIME));
  //Left motor
  analogWrite(A0,  MAXPWM*PCT75);
  analogWrite(A1,   0);
  //Right Motor
  analogWrite(A2,   MAXPWM*PCT75);
  analogWrite(A3,   0);
  delay(secToMs(WAITTIME));
  //Left motor
  analogWrite(A0,  MAXPWM);
  analogWrite(A1,   0);
  //Right Motor
  analogWrite(A2,   MAXPWM);
  analogWrite(A3,   0);
  delay(secToMs(WAITTIME));

  //////////////
  // BACKWARD //
  //////////////
  analogWrite(A0,  0);
  analogWrite(A1,   MAXPWM*PCT25);
  //Right Motor
  analogWrite(A2,   0);
  analogWrite(A3,   MAXPWM*PCT25);
  delay(secToMs(WAITTIME));
  //Left motor
  analogWrite(A0,  0);
  analogWrite(A1,   MAXPWM*PCT75);
  //Right Motor
  analogWrite(A2,   0);
  analogWrite(A3,   MAXPWM*PCT75);
  delay(secToMs(WAITTIME));

}

void task2() {
  // TASK 2 - 25%, 75% Both opposite dirn
  //Left motor - 25% - forward
  analogWrite(A0,  MAXPWM*PCT25);
  analogWrite(A1,   0);
  //Right motor - 25% - backwards
  analogWrite(A2,   0);
  analogWrite(A3,   MAXPWM*PCT25);
  delay(secToMs(WAITTIME));

  //Left motor - 25% - backwards
  analogWrite(A0,  0);
  analogWrite(A1,   MAXPWM*PCT25);
  //Right motor - 25% - forwards
  analogWrite(A2,   MAXPWM*PCT25);
  analogWrite(A3,   0);
  delay(secToMs(WAITTIME));

  //Left motor - 75%  - forward
  analogWrite(A0,  MAXPWM*PCT75);
  analogWrite(A1,   0);
  //Right motor - 75% - backwards
  analogWrite(A2,   0);
  analogWrite(A3,   MAXPWM*PCT75);
  delay(secToMs(WAITTIME));

  //Left motor - 75%  - forward
  analogWrite(A0,  0);
  analogWrite(A1,   MAXPWM*PCT75);
  //Right motor - 75% - backwards
  analogWrite(A2,   MAXPWM*PCT75);
  analogWrite(A3,   0);
  delay(secToMs(WAITTIME));
}