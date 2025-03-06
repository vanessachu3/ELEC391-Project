/**********************************************************************
This code is meant to test hardware timers on the NRF2480 using an Arduino
Nano 33 BLE. It uses the following libraries created by Khoi Hoang:

https://github.com/khoih-prog/nRF52_MBED_PWM
https://github.com/khoih-prog/NRF52_MBED_TimerInterrupt
**********************************************************************/

#if !( ARDUINO_ARCH_NRF52840 && TARGET_NAME == ARDUINO_NANO33BLE )
  #error This code is designed to run on nRF52-based Nano-33-BLE boards using mbed-RTOS platform! Please check your Tools->Board setting.
#endif

// For special ISR debugging .. may cause hangs if >0
#define _PWM_LOGLEVEL_  0

#include <Arduino.h>
#include <nRF52_MBED_PWM.h>
#include <NRF52_MBED_TimerInterrupt.h>

// TimerInterrupt defines, variables, functions
#define TIMER_INTERRUPT_DEBUG     0
#define _TIMERINTERRUPT_LOGLEVEL_ 0

#include <NRF52_MBED_ISR_Timer.h>

#define HW_TIMER_INTERVAL_MS      1

NRF52_MBED_Timer ITimer(NRF_TIMER_3);
NRF52_MBED_ISRTimer ISR_Timer;

#define TIMER_INTERVAL_1S         1000L
#define TIMER_INTERVAL_2S         2000L
#define TIMER_INTERVAL_5S         5000L

void TimerHandler()
{
  ISR_Timer.run();
}

void doingSomething1()
{
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, LOW);
}

void doingSomething2()
{
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
}

void doingSomething3()
{
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, LOW);
}


#define LED_ON        LOW
#define LED_OFF       HIGH

// All the digital pins on Arduino Nano 33 BLE sense are PWM-enabled pins which are numbered from D0 to D13
// Pins D10 and D9 are for motorL, while D8 and D7 are for motorR
uint32_t pins[]       = { D10, D9, D8, D7 };  

#define NUM_OF_PINS     ( sizeof(pins) / sizeof(uint32_t) )

float dutyCycle[]     = { 50.0f, 50.0f, 50.0f, 50.0f };
float freq[]          = { 1000.0f, 2500.0f, 4000.0f, 5000.0f };

float curDutyCycle[]  = { 50.0f, 50.0f, 50.0f, 50.0f };
float curFreq[]       = { 1000.0f, 2500.0f, 4000.0f, 5000.0f };

mbed::PwmOut* pwm[]   = { NULL, NULL, NULL, NULL };

void startAllPWM()
{
  // digitalWrite(LEDG, LED_ON);
  // digitalWrite(LEDB, LED_OFF);
  // digitalWrite(LEDR, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    PWM_LOGERROR7("Freq = ", freq[index], ", \tDutyCycle % = ", dutyCycle[index], ", \tDutyCycle = ",
                  dutyCycle[index] / 100, ", \tPin = ", pins[index]);

    // setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle)
    setPWM(pwm[index], pins[index], freq[index], dutyCycle[index]);
  }
}

void restoreAllPWM()
{
  // digitalWrite(LEDG, LED_ON);
  // digitalWrite(LEDB, LED_OFF);
  // digitalWrite(LEDR, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    curFreq[index]      = freq[index];
    curDutyCycle[index] = dutyCycle[index];

    // setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle)
    setPWM(pwm[index], pins[index], freq[index], dutyCycle[index]);
  }
}

void changeAllPWM()
{
  // digitalWrite(LEDG, LED_OFF);
  // digitalWrite(LEDB, LED_ON);
  // digitalWrite(LEDR, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    curFreq[index]      = freq[index] * 2;
    curDutyCycle[index] = dutyCycle[index] / 2;

    // setPWM(mbed::PwmOut* &pwm, pin_size_t pin, float frequency, float dutyCycle)
    setPWM(pwm[index], pins[index], curFreq[index], curDutyCycle[index]);
  }
}

void stopAllPWM()
{
  // digitalWrite(LEDG, LED_OFF);
  // digitalWrite(LEDB, LED_OFF);
  // digitalWrite(LEDR, LED_ON);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    curFreq[index]      = 1000.0f;
    curDutyCycle[index] = 0.0f;

    //stopPWM(mbed::PwmOut* &pwm, pin_size_t pin)
    stopPWM(pwm[index], pins[index]);
  }
}

void printLine()
{
  Serial.println(F("\n========================================================="));
}

void printPulseWidth()
{
  static uint32_t num = 0;

  if (num++ % 50 == 0)
  {
    printLine();

    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      Serial.print(F("PW (us) "));
      Serial.print(index);
      Serial.print(F("\t"));
    }

    printLine();
  }

  if (num > 1)
  {
    for (uint8_t index = 0; index < NUM_OF_PINS; index++)
    {
      if (pwm[index])
      {
        Serial.print(getPulseWidth_uS(pwm[index]));
        Serial.print(F("\t\t"));
      }
    }

    Serial.println();
  }
}

#define PRINT_INTERVAL      10000L
#define CHANGE_INTERVAL     20000L

void check_status()
{
  static unsigned long checkstatus_timeout  = 0;
  static unsigned long changePWM_timeout    = 0;

  static bool PWM_orig  = true;
  static uint32_t count = 0;

  // Print every PRINT_INTERVAL (10) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    printPulseWidth();
    checkstatus_timeout = millis() + PRINT_INTERVAL;
  }

  if ( (millis() > changePWM_timeout) && (millis() > CHANGE_INTERVAL) )
  {

    if (PWM_orig)
    {
      if (count++ % 2 == 0)
      {
        Serial.println("Stop all PWM");
        stopAllPWM();
      }
      else
      {
        Serial.println("Change all PWM");

        changeAllPWM();

        PWM_orig = !PWM_orig;
      }
    }
    else
    {
      Serial.println("Restore all PWM");

      restoreAllPWM();

      PWM_orig = !PWM_orig;
    }

    changePWM_timeout = millis() + CHANGE_INTERVAL;
  }
}

void setup()
{
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDR, OUTPUT);

  digitalWrite(LEDG, LED_OFF);
  digitalWrite(LEDB, LED_OFF);
  digitalWrite(LEDR, LED_OFF);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    pinMode(pins[index], OUTPUT);
    digitalWrite(pins[index], LOW);
  }

  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(100);

  Serial.print(F("\nStarting PWM_Multi on "));
  Serial.println(BOARD_NAME);
  Serial.println(nRF52_MBED_PWM_VERSION);

  startAllPWM();

  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS*1000, TimerHandler))
  {
    Serial.print(F("Starting ITimer OK, millis() = "));
    Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));

  ISR_Timer.setInterval(TIMER_INTERVAL_1S, doingSomething1);
  ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2);
  ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething3);
}

void loop()
{
  check_status();
}