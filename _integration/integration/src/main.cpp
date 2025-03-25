#if !( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
  #error This code is designed to run on nRF52-based Nano-33-BLE boards using mbed-RTOS platform! Please check your Tools->Board setting.
#endif

#include <Arduino.h>

//==========================================================
// PID
//==========================================================
#include <PID.h>

//==========================================================
// PWM
//==========================================================
#include <PWM.h>
#include <nRF52_MBED_PWM.h>

//==========================================================
// Signal variables/functions
//==========================================================

void SIG_init();

// timer-based functions
void sigL();
void sigR();

// turn signal GPIO pins
#define pinL    D2
#define pinR    D3

// global flags for turn signals
bool flagL = 0;
bool flagR = 0;
bool flagH = 0;

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

void sigH()
{
    if(flagH)
    {
        flagL = 1;
        flagR = 1;
    }
    else if(!flagH && flagL)
        flagR = 0;
    else if(!flagH && flagR)
        flagL = 0;
}

//==========================================================
// IMU variables/functions
//==========================================================
#include <Arduino_BMI270_BMM150.h>

void IMU_init();
float getAngle();

#define angleOffset 0.04
#define k           0.79  // filter coefficient

float ax, ay, az;
float gx, gy, gz;
float prevAngle   = 0;

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

        return currAngle + angleOffset;
    }
    else
        return 0;
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

//==========================================================
// BLE
//==========================================================
// #include <BLE.h>

// variables
float startMillis;
float currMillis;

void setup() 
{
  Serial.begin(115200);

  SIG_init();
  PWM_init();
  TIMERF_init();
  // BLE_init();
  IMU_init();

  currMillis = millis();

  flagL = 0;
  flagR = 1;
}

void loop()
{
  // insert commands here to test w/o connecting to BLE
  float currAngle = getAngle();
  currMillis = (millis() - currMillis)/1000.0f; // in seconds
  PID_update(&pid, currAngle, currMillis);
  
  // Wait for  a BLE central to connect
  #if 0
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
  #endif 
}