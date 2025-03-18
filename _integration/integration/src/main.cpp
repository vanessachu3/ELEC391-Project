#if !( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
  #error This code is designed to run on nRF52-based Nano-33-BLE boards using mbed-RTOS platform! Please check your Tools->Board setting.
#endif

#include <Arduino.h>

// pid
#include <PID.h>

// pwm headers
#include <PWM.h>
#include <nRF52_MBED_PWM.h>

// timers - timers must be initialized in main()
#include <TIMERS.h>
#include <NRF52_MBED_TimerInterrupt.h>
#include <NRF52_MBED_ISR_Timer.h>

#define TIMER_INTERRUPT_DEBUG     0
#define _TIMERINTERRUPT_LOGLEVEL_ 0

#define HW_TIMER_INTERVAL_100us     100   // in micro-seconds
#define TIMER_INTERVAL_100us        0.1   // in milli-seconds

NRF52_MBED_Timer ITimer(NRF_TIMER_3);
NRF52_MBED_ISRTimer ISR_Timer;

void TimerHandler()
{
    ISR_Timer.run();
}

void TIMERS_init()
{
    if(ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_100us, TimerHandler))
    {
        Serial.print(F("ITimer started ..."));
    }
    else
        Serial.println(F("Starting ITimer failed!"));

    // ISR_Timer.setInterval(TIMER_INTERVAL_100us, getOutput);
    // ISR_Timer.setInterval(TIMER_INTERVAL_100us, PID_update);

    // turn on LEDB to indicate timers are active
    // pinMode(LEDR, OUTPUT);
    // pinMode(LEDB, OUTPUT);
    // digitalWrite(LEDR, LOW);
}

// ble
#include <BLE.h>

// variables
volatile float test = 0.5; // should be a % (0 - 100)
float startMillis;
float currMillis;


void setup() 
{
  // pinMode(LEDR, OUTPUT);
  // pinMode(LEDG, OUTPUT);
  // pinMode(LEDB, OUTPUT);

  // digitalWrite(LEDR, LOW);
  // digitalWrite(LEDG, LOW);
  // digitalWrite(LEDB, LOW);

  Serial.begin(115200);

  PWM_init(); 
  TIMERS_init();
  BLE_init();
  IMU_init();

  currMillis = millis();
}

void loop()
{
  // insert commands here to test w/o connecting to BLE
  float currAngle = getAngle();
  currMillis = (millis() - currMillis)/1000.0f;
  PID_update(&pid, currAngle, currMillis);
  
  // Wait for a BLE central to connect
  BLEDevice central = BLE.central();

  if(central) 
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate connection

    // Keep running while connected
    while (central.connected()) 
    { 
      // Check if the characteristic was written
      if (customCharacteristic.written())
      {
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


        // Optionally, respond by updating the characteristic's value
        customCharacteristic.writeValue("Data received");
      }

      // insert commands here to be run while connected to BLE
    }

    digitalWrite(LED_BUILTIN, LOW); // Turn off LED when disconnected
    Serial.println("Disconnected from central.");
    PWM_stop();
  } 
}