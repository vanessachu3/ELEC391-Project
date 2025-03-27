#if 1
#include <ArduinoBLE.h>
#include <BLESetup.h>
#endif
#include <Arduino.h>
#include <PWM.h>
#include <balanceRobot.h>
#include <pid.h>
#include <actuator.h>
#include <audio.h>

#if 1
//==========================================================
// Signal variables/functions
//==========================================================

void SIG_init();

// timer-based functions
void sigL();
void sigR();
void sigOff();

// turn signal GPIO pins
#define pinL    D4
#define pinR    D3

// global flags for turn signals
bool flagL = 0;
bool flagR = 0;

void SIG_init()
{
    pinMode(pinL, OUTPUT);
    pinMode(pinR, OUTPUT);
}

void sigL()
{
    if(flagL)
        digitalWrite(pinL, !digitalRead(pinL));
    else
        digitalWrite(pinL, LOW);
}

void sigR()
{
    if(flagR)
        digitalWrite(pinR, !digitalRead(pinR));
    else
        digitalWrite(pinR, LOW);
}

void sigOff()
{
  flagL = 0;
  flagR = 0;
}
//==========================================================
// TIMERS (must be initialized in main.cpp)
//==========================================================
#include <NRF52_MBED_TimerInterrupt.h>
#include <NRF52_MBED_ISR_Timer.h>

void TimerHandler();
void TIMERF_init();

#define TIMER_INTERRUPT_DEBUG       0
// #define _TIMERINTERRUPT_LOGLEVEL_ 0

#define HW_TIMER_INTERVAL_100us     100L  // in micro-seconds
#define TIMER_INTERVAL_500ms        500L  // in milli-seconds

NRF52_MBED_Timer ITimer(NRF_TIMER_3);
NRF52_MBED_ISRTimer ISR_Timer;

void TimerHandler()
{
    ISR_Timer.run();
}

void TIMERF_init()
{
    if(!ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_100us, TimerHandler))
    {
        Serial.print("Failed to start ITimer!");
        pinMode(LEDR, OUTPUT);
        digitalWrite(LEDR, LOW);  // turn on the red LED if error
    }
    else
        Serial.println("ITimer started...");

    ISR_Timer.setInterval(TIMER_INTERVAL_500ms, sigL);
    ISR_Timer.setInterval(TIMER_INTERVAL_500ms, sigR);
}
#endif
float gyroTs = 0.01;
float currMillis;
void setup() {
  setupBLE();
  gyroTs = getAngleSetup();
  currMillis = millis();
  SIG_init();
  TIMERF_init();
}

void loop() {
  // Wait for a BLE central to connect
  BLEDevice central = BLE.central();
  
  float sampleSec = (millis() - currMillis)/1000.0f;
  currMillis = millis();
  //Serial.println(sampleSec);
  //TODO: ROBOT SHOULD BE ACTIVELY BALANCING
  float angle = getAngle(sampleSec);
  //Serial.println(angle);
  balance(&pid,angle,sampleSec);
  processSerialInput(&pid);
  //Serial.println(angle);
  
  
#if 1
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate connection

    // Keep running while connected
    while (central.connected()) {

      float sampleSec = (millis() - currMillis)/1000.0f;
      currMillis = millis();
      //Serial.println(sampleSec);
      //TODO: ROBOT SHOULD BE ACTIVELY BALANCING
      float angle = getAngle(sampleSec);
      //Serial.println(angle);
      balance(&pid,angle,sampleSec);
      processSerialInput(&pid);

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

        // Split the received string by the comma
        char* direction = strtok(receivedString, ",");
        char* distanceStr = strtok(NULL, ",");  // Get the distance part
        float distance = atof(distanceStr);
        //TODO: SCALE SPEED WRT JOYSTICK POSITION
        moveRobotCommand(receivedString, distance*255.0, &pid);
        
        //LEFT/RIGHT/HAZARD SIGNALS
        if (strcmp(receivedString, "LEFT SIGNAL") == 0) {
          flagL = 1;
        }
        else if (strcmp(receivedString, "RIGHT SIGNAL") == 0) {
          flagR = 1;
        }
        else if (strcmp(receivedString, "HAZARD") == 0) {
          flagL = 1;
          flagR = 1;
        }
        else if (strcmp(receivedString, "STOP") == 0) {
          sigOff();
        }

        //PLATFROM EXTENSION
        actuatorLoop(receivedString);
        //AUDIO PLAYBACK
        play_music(receivedString);
        // Optionally, respond by updating the characteristic's value
        customCharacteristic.writeValue("Data received");
      }

    }

    digitalWrite(LED_BUILTIN, LOW); // Turn off LED when disconnected
    Serial.println("Disconnected from central.");
  }
  #endif
}