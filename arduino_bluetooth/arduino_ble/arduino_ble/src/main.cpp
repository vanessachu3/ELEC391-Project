#include <Arduino.h>
#include <ArduinoBLE.h>
#include <PWM.h>

#define BUFFER_SIZE 20

// Define a custom BLE service and characteristic
BLEService customService("00000000-5EC4-4083-81CD-A10B8D5CF6EC");
BLECharacteristic customCharacteristic(
    "00000001-5EC4-4083-81CD-A10B8D5CF6EC", BLERead | BLEWrite | BLENotify, BUFFER_SIZE, false);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize the built-in LED to indicate connection status
  pinMode(LED_BUILTIN, OUTPUT);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set the device name and local name
  BLE.setLocalName("BLE-B5");
  BLE.setDeviceName("BLE-B5");

  // Add the characteristic to the service
  customService.addCharacteristic(customCharacteristic);

  // Add the service
  BLE.addService(customService);

  // Set an initial value for the characteristic
  customCharacteristic.writeValue("Waiting for data");

  // Start advertising the service
  //BLE.setAdvertisingInterval(20);
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop() {
  // Wait for a BLE central to connect
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate connection

    // Keep running while connected
    while (central.connected()) {
      // Check if the characteristic was written
      if (customCharacteristic.written()) {
       // Get the length of the received data
        int length = customCharacteristic.valueLength();

        // Read the received data
        const unsigned char* receivedData = customCharacteristic.value();

        // Create a properly terminated string
        char receivedString[length + 1]; // +1 for null terminator
        memcpy(receivedString, receivedData, length);
        receivedString[length] = '\0'; // Null-terminate the string

        // Print the received data to the Serial Monitor
        Serial.print("Received data: ");
        Serial.println(receivedString);
        if (strcmp(receivedString, "FORWARD") == 0) {
          PWMfwrd(1.0);  // Move forward at full speed
        } 
        else if (strcmp(receivedString, "BACKWARDS") == 0) {
          PWMbkwrd(1.0);  // Move backward at full speed
        }
        else if (strcmp(receivedString, "LEFT") == 0) {
          PWMleft(1.0);  // Turn left
        }
        else if (strcmp(receivedString, "RIGHT") == 0) {
          PWMright(1.0);  // Turn right
        }
        else if (strcmp(receivedString, "FORWARD LEFT") == 0) {
          PWMfwrdLeft(1.0);  // fwrd left
        } 
        else if (strcmp(receivedString, "FORWARD RIGHT") == 0) {
          PWMfwrdRight(1.0);  // fwrd right
        }
        else if (strcmp(receivedString, "BACKWARDS LEFT") == 0) {
          PWMbkwrdLeft(1.0);  // bkwrd left
        }
        else if (strcmp(receivedString, "BACKWARDS RIGHT") == 0) {
          PWMbkwrdRight(1.0);  // bkwrd right
        }

        // Optionally, respond by updating the characteristic's value
        customCharacteristic.writeValue("Data received");
      }
    }

    digitalWrite(LED_BUILTIN, LOW); // Turn off LED when disconnected
    Serial.println("Disconnected from central.");
  }
}