// Rx button commands then execute appropriate motions

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Arduino_BMI270_BMM150.h"


#define MAXPWM 255
#define BAUD 115200
#define LEFTWHEEL_FWRD    A0
#define LEFTWHEEL_BKWRD   A1
#define RIGHTWHEEL_FWRD   A2
#define RIGHTWHEEL_BKWRD  A3


void PWMfwrd(float scaleFactor);
void PWMbkwrd(float scaleFactor);
void PWMleft(float scaleFactor);
void PWMright(float scaleFactor);
void PWMfwrdLeft(float scaleFactor);
void PWMfwrdRight(float scaleFactor);
void PWMbkwrdLeft(float scaleFactor);
void PWMbkwrdRight(float scaleFactor);

void PWMfwrd(float scaleFactor) {
  
  if (scaleFactor >=1)
  {
    //Left motor
    analogWrite(LEFTWHEEL_FWRD,  MAXPWM);
    analogWrite(LEFTWHEEL_BKWRD,   0);

    //Right Motor
    analogWrite(RIGHTWHEEL_FWRD,   MAXPWM);
    analogWrite(RIGHTWHEEL_BKWRD,   0);
  }
  else
  {
    //Left Motor
    analogWrite(LEFTWHEEL_FWRD,  MAXPWM*scaleFactor);
    analogWrite(LEFTWHEEL_BKWRD,   0);

    //Right Motor
    analogWrite(RIGHTWHEEL_FWRD,   MAXPWM*scaleFactor);
    analogWrite(RIGHTWHEEL_BKWRD,   0);}
    Serial.println("GOING FORWARD");
}

void PWMbkwrd(float scaleFactor) {
  if (scaleFactor >=1)
  {
    //Left motor
    analogWrite(LEFTWHEEL_FWRD,  0);
    analogWrite(LEFTWHEEL_BKWRD,   MAXPWM);

    //Right motor
    analogWrite(RIGHTWHEEL_FWRD,   0);
    analogWrite(RIGHTWHEEL_BKWRD,   MAXPWM);
  }
  else{
    //Left motor
    analogWrite(LEFTWHEEL_FWRD,  0);
    analogWrite(LEFTWHEEL_BKWRD,   MAXPWM*scaleFactor);

    //Right motor
    analogWrite(RIGHTWHEEL_FWRD,   0);
    analogWrite(RIGHTWHEEL_BKWRD,   MAXPWM*scaleFactor);
  }
  Serial.println("GOING BACKWARD");
}

void PWMleft(float scaleFactor) {
    if (scaleFactor >=1)
    {
      //Left motor
      analogWrite(LEFTWHEEL_FWRD,  0);
      analogWrite(LEFTWHEEL_BKWRD,   MAXPWM);
  
      //Right motor
      analogWrite(RIGHTWHEEL_FWRD,   MAXPWM);
      analogWrite(RIGHTWHEEL_BKWRD,   0);
    }
    else{
      //Left motor
      analogWrite(LEFTWHEEL_FWRD,  0);
      analogWrite(LEFTWHEEL_BKWRD,   MAXPWM*scaleFactor);
  
      //Right motor
      analogWrite(RIGHTWHEEL_FWRD,   MAXPWM*scaleFactor);
      analogWrite(RIGHTWHEEL_BKWRD,   0);
    }
    Serial.println("GOING LEFT");
}

void PWMright(float scaleFactor) {
    if (scaleFactor >=1)
    {
      //Left motor
      analogWrite(LEFTWHEEL_FWRD,  MAXPWM);
      analogWrite(LEFTWHEEL_BKWRD,   MAXPWM);
  
      //Right motor
      analogWrite(RIGHTWHEEL_FWRD,   0);
      analogWrite(RIGHTWHEEL_BKWRD,   MAXPWM);
    }
    else{
      //Left motor
      analogWrite(LEFTWHEEL_FWRD,  MAXPWM*scaleFactor);
      analogWrite(LEFTWHEEL_BKWRD,   0);
  
      //Right motor
      analogWrite(RIGHTWHEEL_FWRD,   0);
      analogWrite(RIGHTWHEEL_BKWRD,   MAXPWM*scaleFactor);
    }
    Serial.println("GOING RIGHT");
}

void PWMfwrdLeft(float scaleFactor) {
    if (scaleFactor >= 1) {
      analogWrite(LEFTWHEEL_FWRD, MAXPWM * 0.5);  // Left wheel slower
      analogWrite(LEFTWHEEL_BKWRD, 0);
      analogWrite(RIGHTWHEEL_FWRD, MAXPWM);       // Right wheel full speed
      analogWrite(RIGHTWHEEL_BKWRD, 0);
    } else {
      analogWrite(LEFTWHEEL_FWRD, MAXPWM * 0.5 * scaleFactor);
      analogWrite(LEFTWHEEL_BKWRD, 0);
      analogWrite(RIGHTWHEEL_FWRD, MAXPWM * scaleFactor);
      analogWrite(RIGHTWHEEL_BKWRD, 0);
    }
    Serial.println("GOING FORWARD LEFT");
  }
  
void PWMfwrdRight(float scaleFactor) {
    if (scaleFactor >= 1) {
      analogWrite(LEFTWHEEL_FWRD, MAXPWM);       // Left wheel full speed
      analogWrite(LEFTWHEEL_BKWRD, 0);
      analogWrite(RIGHTWHEEL_FWRD, MAXPWM * 0.5); // Right wheel slower
      analogWrite(RIGHTWHEEL_BKWRD, 0);
    } else {
      analogWrite(LEFTWHEEL_FWRD, MAXPWM * scaleFactor);
      analogWrite(LEFTWHEEL_BKWRD, 0);
      analogWrite(RIGHTWHEEL_FWRD, MAXPWM * 0.5 * scaleFactor);
      analogWrite(RIGHTWHEEL_BKWRD, 0);
    }
    Serial.println("GOING FORWARD RIGHT");
  }
  
void PWMbkwrdLeft(float scaleFactor) {
    if (scaleFactor >= 1) {
      analogWrite(LEFTWHEEL_FWRD, 0);
      analogWrite(LEFTWHEEL_BKWRD, MAXPWM * 0.5);  // Left wheel slower
      analogWrite(RIGHTWHEEL_FWRD, 0);
      analogWrite(RIGHTWHEEL_BKWRD, MAXPWM);       // Right wheel full speed
    } else {
      analogWrite(LEFTWHEEL_FWRD, 0);
      analogWrite(LEFTWHEEL_BKWRD, MAXPWM * 0.5 * scaleFactor);
      analogWrite(RIGHTWHEEL_FWRD, 0);
      analogWrite(RIGHTWHEEL_BKWRD, MAXPWM * scaleFactor);
    }
    Serial.println("GOING BACKWARD LEFT");
  }
  
void PWMbkwrdRight(float scaleFactor) {
    if (scaleFactor >= 1) {
      analogWrite(LEFTWHEEL_FWRD, 0);
      analogWrite(LEFTWHEEL_BKWRD, MAXPWM);       // Left wheel full speed
      analogWrite(RIGHTWHEEL_FWRD, 0);
      analogWrite(RIGHTWHEEL_BKWRD, MAXPWM * 0.5); // Right wheel slower
    } else {
      analogWrite(LEFTWHEEL_FWRD, 0);
      analogWrite(LEFTWHEEL_BKWRD, MAXPWM * scaleFactor);
      analogWrite(RIGHTWHEEL_FWRD, 0);
      analogWrite(RIGHTWHEEL_BKWRD, MAXPWM * 0.5 * scaleFactor);
    }
    Serial.println("GOING BACKWARD RIGHT");
  }