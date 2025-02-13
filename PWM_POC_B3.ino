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
// % valuaes of max PWM
// float dutyCycleA0  = 1.0;
// float dutyCycleA1   = 0.75;
// float dutyCycleA2   = 0.5;
// float dutyCycleA3   = 0.25;

float ax, ay, az, gx, gy, gz;                           // IMU acceleration | IMU gyroscope
float accAngle, gyrAngle = 0, currAngle, prevAngle = 0; // initialize accAngle and gyrAngle
float gyrPrev = 0;                                      // initialize gyroscope integration constant
float gyrSampleRate;                                    // gyroscope sample rate

float k = 0.8;                                          // filter coefficient

void PWMfwrd(float scaleFactor);
void PWMbkwrd(float scaleFactor);

void setup() {

  pinMode(A0, OUTPUT); 
  pinMode(A1,  OUTPUT);
                                                                                                                                            
  pinMode(A2,  OUTPUT);
  pinMode(A3,  OUTPUT);

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

  task1();
  task2();
  task3();
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
  }

}



void PWMfwrd(float scaleFactor) {
  
  if (scaleFactor >=1)
  {
    //Left motor
    analogWrite(A0,  MAXPWM);
    analogWrite(A1,   0);

    //Right Motor
    analogWrite(A2,   MAXPWM);
    analogWrite(A3,   0);
  }
  else
  {
    //Left Motor
    analogWrite(A0,  MAXPWM*scaleFactor);
    analogWrite(A1,   0);

    //Right Motor
    analogWrite(A2,   MAXPWM*scaleFactor);
    analogWrite(A3,   0);}
}

void PWMbkwrd(float scaleFactor) {
  if (scaleFactor >=1)
  {
    //Left motor
    analogWrite(A0,  0);
    analogWrite(A1,   MAXPWM);

    //Right motor
    analogWrite(A2,   0);
    analogWrite(A3,   MAXPWM);
  }
  else{
    //Left motor
    analogWrite(A0,  0);
    analogWrite(A1,   MAXPWM*scaleFactor);

    //Right motor
    analogWrite(A2,   0);
    analogWrite(A3,   MAXPWM*scaleFactor);
  }

}