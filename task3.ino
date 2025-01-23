// Angle-sensing, measurement and display using the
// Arduino Nano BLE Sense IMU.

// Task 3 | Computing angles with gyro readings

#include <stdio.h>
#include <stdlib.h>
#include "Arduino_BMI270_BMM150.h"

#define BAUD 115200

int data = 0;
char userInput;

float gx, gy, gz;           // IMU gyroscope variables

void setup()
{
    Serial.begin(BAUD);
    Serial.println("Task 1 | Data Plotting")

    if(!IMU.begin())        // IMU initialization
    {
        Serial.println("Failed to initialize IMU!");
        while(1);
    }


    Serial.print("Gyroscope Sample Rate: ");
    Serial.print(IMU.gyroscopeSampleRate());
    Serial.println(" Hz");
    Serial.println();
}

void loop() {
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