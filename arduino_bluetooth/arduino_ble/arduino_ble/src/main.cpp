#include <Arduino.h>
#include <ArduinoBLE.h>
#include <PWM.h>
#include <BLESetup.h>
#include <balanceRobot.h>
#include <pid.h>


float gyroTs = 0.01;
void setup() {
  setupBLE();
  gyroTs = getAngleSetup();
}

void loop() {
  // Wait for a BLE central to connect
  BLEDevice central = BLE.central();

  //TODO: ROBOT SHOULD BE ACTIVELY BALANCING
  float angle = getAngle(gyroTs);
  //Serial.println(angle);
  balance(&pid,angle);

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate connection


    // Keep running while connected
    // while (central.connected()) {
    //   // Check if the characteristic was written
    //   if (customCharacteristic.written()) {
    //    // Get the length of the received data
    //     int length = customCharacteristic.valueLength();

    //     // Read the received data
    //     const unsigned char* receivedData = customCharacteristic.value();

    //     // Create a properly terminated string
    //     char receivedString[length + 1]; // +1 for null terminator
    //     memcpy(receivedString, receivedData, length);
    //     receivedString[length] = '\0'; // Null-terminate the string

    //     // Print the received data to the Serial Monitor
    //     Serial.print("Received data: ");
    //     Serial.println(receivedString);

    //     // Split the received string by the comma
    //     char* direction = strtok(receivedString, ",");
    //     char* distanceStr = strtok(NULL, ",");  // Get the distance part
    //     float distance = atof(distanceStr);
    //     //TODO: SCALE SPEED WRT JOYSTICK POSITION
    //     //moveRobot(receivedString, distance);

    //     //TODO: ADD LEFT/RIGHT/HAZARD SIGNALS
        

    //     // Optionally, respond by updating the characteristic's value
    //     customCharacteristic.writeValue("Data received");
    //   }
    //   else
    //   {
        
    //   }
    // }

    digitalWrite(LED_BUILTIN, LOW); // Turn off LED when disconnected
    Serial.println("Disconnected from central.");
  }
}