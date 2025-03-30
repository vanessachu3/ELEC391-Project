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

float ax, ay, az, gx, gy, gz;                           // IMU acceleration | IMU gyroscope
float accAngle, gyrAngle = 0, currAngle, prevAngle = 0; // initialize accAngle and gyrAngle
float gyrPrev = 0;                                      // initialize gyroscope integration constant
float gyrSampleRate;                                    // gyroscope sample rate

float k = 0.8;                                          // filter coefficient

void PWMfwrd(float scaleFactor);
void PWMbkwrd(float scaleFactor);

void setup() {

  pinMode(LEFTWHEEL_FWRD, OUTPUT); 
  pinMode(LEFTWHEEL_BKWRD,  OUTPUT);
                                                                                                                                            
  pinMode(RIGHTWHEEL_FWRD,  OUTPUT);
  pinMode(RIGHTWHEEL_BKWRD,  OUTPUT);

  Serial.begin(BAUD);
  if(!IMU.begin())                                    // IMU initialization
    {
        Serial.println("Failed to initialize IMU!");
        while(1);
    }

    gyrSampleRate = IMU.gyroscopeSampleRate();          // acquire sample rate for gyroscope angle
  
}

void loop() {
  // PWM Outputs
  //PWMOutput();
  // check for IMU acceleration and gyroscope
  if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable())
  {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);

    // calculate accelerometer angle
    accAngle = atan2(ay, az)*180/PI;

    // calculate gyroscope angle
    gyrAngle = gyrPrev - gx/gyrSampleRate;
    if(accAngle < 0.25 && accAngle > -0.25)
    {
      gyrAngle = 0;
    }
    gyrPrev = gyrAngle;

    // calculate filtered angle
    currAngle = k*(prevAngle - gx/gyrSampleRate)+(1-k)*accAngle;
    prevAngle = currAngle; 
    Serial.println(currAngle);
  }
  
  if (currAngle >=-1 && currAngle <=1)
  {
    PWMfwrd(0)
  }
  else
  {
  //if angle negative - drive backwards
  if (currAngle < 0)
  {
    //PWMbkwrd()
    PWMbkwrd(currAngle/30 * -1);
  }
  //if angle positive - drive fwrds
  else 
  {
    PWMfwrd(currAngle/30 * 1);
  }}

}



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

}