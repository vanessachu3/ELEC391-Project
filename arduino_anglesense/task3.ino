/**********************************************************************

Angle-sensing, measurement and display using the
Arduino Nano BLE Sense IMU.

Future integrations of this code will include PID for balance control
For BLE applications, Timer 3 and 4 are the only stable options [https://forum.arduino.cc/t/five-hardware-timers-example/905798/2]
nRF52 timers [https://medium.com/vicara-hardware-university/using-timers-on-nrf52-b0497f0633a1] [https://docs.arduino.cc/libraries/nrf52_timerinterrupt/]
nRF52 PWM [https://docs.nordicsemi.com/bundle/ps_nrf52840/page/pwm.html] [https://academy.nordicsemi.com/courses/nrf-connect-sdk-intermediate/lessons/lesson-4-pulse-width-modulation-pwm/topic/pulse-width-modulation-pwm/]

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Arduino_BMI270_BMM150.h"

#define BAUD 115200

float ax, ay, az, gx, gy, gz;                           // IMU acceleration | IMU gyroscope
float accAngle, gyrAngle = 0, currAngle, prevAngle = 0; // initialize accAngle and gyrAngle
float gyrPrev = 0;                                      // initialize gyroscope integration constant
float gyrSampleRate;                                    // gyroscope sample rate

float k = 0.8;                                          // filter coefficient

void setup()
{
    Serial.begin(BAUD);
    Serial.println("Calculating Angles based on Accelerometer/Gyroscope Data");

    if(!IMU.begin())                                    // IMU initialization
    {
        Serial.println("Failed to initialize IMU!");
        while(1);
    }

    gyrSampleRate = IMU.gyroscopeSampleRate();          // acquire sample rate for gyroscope angle
}

void loop() 
{
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

    Serial.print(accAngle);
    Serial.print(" ");
    Serial.print(gyrAngle);
    Serial.print(" ");
    Serial.println(currAngle);
  }
}
  