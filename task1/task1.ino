// Angle-sensing, measurement and display using the
// Arduino Nano BLE Sense IMU.

// Task 1 | Data Plotting

#include <stdio.h>
#include <stdlib.h>
#include "Arduino_BMI270_BMM150.h"

#define BAUD 115200

int data = 0;

float ax, ay, az;           // IMU acceleration variables
float gx, gy, gz;           // IMU gyroscope variables

void setup()
{
    Serial.begin(BAUD);
    Serial.println("Task 1 | Data Plotting");

    if(!IMU.begin())        // IMU initialization
    {
        Serial.println("Failed to initialize IMU!");
        while(1);
    }

    Serial.print("Accelerometer Sample Rate: ");
    Serial.print(IMU.accelerationSampleRate());
    Serial.println(" Hz");
    Serial.println();

    Serial.print("Gyroscope Sample Rate: ");
    Serial.print(IMU.gyroscopeSampleRate());  
    Serial.println(" Hz");
    Serial.println();
}

void loop() {
    if(IMU.accelerationAvailable())
    {
        IMU.readAcceleration(ax, ay, az);
        Serial.print(ax);
        Serial.print('\t');
        Serial.print(ay);
        Serial.print('\t');
        Serial.print(az);
        Serial.print('\t');
    }

     if(IMU.gyroscopeAvailable())
    {
        IMU.readGyroscope(gx, gy, gz);
        //Serial.println("Gyroscope readings:");
        Serial.print(gx);
        Serial.print('\t');
        Serial.print(gy);
        Serial.print('\t');
        Serial.println(gz);
    }
}