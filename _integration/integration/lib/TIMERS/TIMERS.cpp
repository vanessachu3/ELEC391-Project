/**********************************************************************
TIMERS will be responsible for both initializing/calling ISR timers, and
data acquisition/processing for the robot angle and PID control. A SW
timer will be used for non mission-critical tasks, like speaker/LEDs.
**********************************************************************/

#include "TIMERS.h"
#include "PID.h"
#include "Arduino_BMI270_BMM150.h"
#include <Arduino.h>

// turn signals

float ax, ay, az, gx, gy, gz, prevAngle = 0;

// k is the filter coefficient; can be modified
static float k              = 0.77;

void IMU_init()
{
    if(!IMU.begin())
    {
        Serial.println("Failed to initialize IMU!");
        while(1);
    }
}

float getAngle()
{
    if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable())
    {
        float gyrSampleRate = IMU.gyroscopeSampleRate();

        IMU.readAcceleration(ax, ay, az);
        IMU.readGyroscope(gx, gy, gz);

        float accAngle = atan(ay/az)*180/PI;
        float gyrAngle = prevAngle - gx/gyrSampleRate;

        float currAngle = k*gyrAngle + (k-1)*accAngle;
        prevAngle = currAngle;

        return currAngle;
    }
    else
        return 0;
}