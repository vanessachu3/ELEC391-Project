// Angle-sensing, measurement and display using the
// Arduino Nano BLE Sense IMU.

// Task 2 | Computing angles with accelerometer readings

#include <stdio.h>
#include <stdlib.h>
#include "Arduino_BMI270_BMM150.h"

#define BAUD 115200

int data = 0;
char userInput;

float ax, ay, az;           // IMU acceleration variables   

void setup()
{
    Serial.begin(BAUD);
    Serial.println("Task 1 | Data Plotting")

    if(!IMU.begin())        // IMU initialization
    {
        Serial.println("Failed to initialize IMU!");
        while(1);
    }

    Serial.print("Accelerometer Sample Rate: ");
    Serial.print(IMU.accelerationSampleRate());
    Serial.println(" Hz");
    Serial.println();

}

void loop() {
    if(IMU.accelerationAvailable())
    {
        IMU.readAcceleration(ax, ay, az);
        //Serial.println("Accelerometer readings:");
        Serial.print(ax);
        Serial.print('\t');
        Serial.print(ay);
        Serial.print('\t');
        Serial.println(az);
    }

}