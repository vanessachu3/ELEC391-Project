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
#include <oled.h>
#include <Arduino_APDS9960.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#if 1
//==========================================================
// Music variables/functions
//==========================================================
volatile bool musicFlag = false;
int noteIndex = 0;
int ice_cream_song[] = {
  NOTE_C5, NOTE_AS4, 
  NOTE_GS4, NOTE_GS4, NOTE_AS4, 
  NOTE_GS4, NOTE_DS4, NOTE_C4, NOTE_CS4,
  NOTE_DS4, NOTE_F4, NOTE_DS4, NOTE_C4,
  NOTE_DS4, NOTE_GS4, NOTE_AS4,
  NOTE_C5, NOTE_C5,
  NOTE_C5, NOTE_AS4, NOTE_GS4, NOTE_AS4,
  NOTE_C5, NOTE_AS4, 
  NOTE_AS4, NOTE_C5, NOTE_AS4,
  NOTE_GS4, NOTE_GS4, NOTE_AS4,
  NOTE_GS4, NOTE_DS4, NOTE_C4, NOTE_CS4,
  NOTE_DS4, NOTE_F4, NOTE_DS4, NOTE_C4,
  NOTE_DS4, NOTE_GS4, NOTE_AS4,
  NOTE_C5, NOTE_DS5, NOTE_DS5, NOTE_F5,
  NOTE_DS5, NOTE_C5, NOTE_GS4, NOTE_AS4,
  NOTE_C5, NOTE_AS4, 
  NOTE_GS4
};
// 4 = quarter note, 8 = eighth note, etc.
int note_duration[] = {
  8, 8,
  4, 8, 8,
  8, 8, 8, 8,
  8, 8, 8, 8,
  4, 8, 8,
  4, 4,
  8, 8, 8, 8,
  4, 4,
  4, 8, 8,
  4, 8, 8, 
  8, 8, 8, 8,
  8, 8, 8, 8,
  4, 8, 8,
  8, 8, 8, 8,
  8, 8, 8, 8,
  4, 4, 
  4
};
bool musicPlaying = false;
bool stopRequested = false;
unsigned long lastNoteTime = 0;
int currentNote = 0;

void playMusicNonBlocking() {
  if (!musicPlaying) return;
  
  unsigned long currentTime = millis();
  
  // If it's time to play the next note
  if (currentTime - lastNoteTime >= (1000/note_duration[currentNote] * 1.30)) {
    noTone(speaker_pin); // Stop previous note
    
    if (stopRequested) {
      musicPlaying = false;
      stopRequested = false;
      currentNote = 0;
      noTone(speaker_pin);
      digitalWrite(speaker_pin, LOW);
      return;
    }
    
    // Play current note
    tone(speaker_pin, ice_cream_song[currentNote], 1000/note_duration[currentNote]);
    
    // Move to next note
    currentNote++;
    if (currentNote >= sizeof(ice_cream_song)/sizeof(ice_cream_song[0])) {
      currentNote = 0; // Loop or set musicPlaying = false to stop
    }
    
    lastNoteTime = currentTime;
  }
}

void play_music(const char* command) {
  if (strcmp(command, "MUSIC_ON") == 0) {
    musicPlaying = true;
    stopRequested = false;
    currentNote = 0;
    lastNoteTime = 0;
  }
  else if (strcmp(command, "MUSIC_OFF") == 0) {
    noTone(speaker_pin);
    if (musicPlaying) {
      stopRequested = true;
    } else {
      noTone(speaker_pin);
    }
  }
}
//==========================================================
// Signal variables/functions
//==========================================================

void SIG_init();

// timer-based functions
void sigL();
void sigR();
void sigOff();

void checkLights();

// turn signal GPIO pins
#define pinL    D4
#define pinR    D3
#define pinH    D2


// global flags for turn signals
bool flagL = 0;
bool flagR = 0;

void SIG_init()
{
    if(!APDS.begin())
    {
      Serial.println("Failed to initialize APDS!");
      while(1);
    }
    
    pinMode(pinL, OUTPUT);
    pinMode(pinR, OUTPUT);
    pinMode(pinH, OUTPUT);
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

#define CHECKLIGHT_MIN 16
void checkLights()      // call checkLights in loop()
{
  if(APDS.colorAvailable())
  {
    int r, g, b, a;
    APDS.readColor(r, g, b, a);

    if(a > 0 && a < CHECKLIGHT_MIN)
      digitalWrite(pinH, HIGH);
    else
      digitalWrite(pinH, LOW);
  }
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
#define TIMER_INTERVAL        350l  // in milli-seconds

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

    ISR_Timer.setInterval(TIMER_INTERVAL, sigL);
    ISR_Timer.setInterval(TIMER_INTERVAL, sigR);
    //ISR_Timer.setInterval(TIMER_MUSIC_INTERVAL, playNote);
}
#endif
float gyroTs = 0.01;
float currMillis;
#define SAMPLEAVG 50

//==========================================================
// turning variables
//==========================================================
bool dfw = false;
bool dbw = false;
bool dlf = false;
bool drt = false;

void setup() {
  Serial.begin(9600);
  delay(200);
  setupBLE();
  gyroTs = getAngleSetup();
  currMillis = millis();
  SIG_init();
  TIMERF_init();
  audioSetup();
  actuatorSetup();
  ///////////////////
  //     OLED      //
  ///////////////////
  #if 1
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  #endif

  //musicFlag = true;
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  float kp = getKp( & pid);
  float ki = getKi( & pid);
  float kd = getKd( & pid);
  display.print(F("Kp: "));
  display.println(kp);
  display.print(F("Ki: "));
  display.println(ki);
  display.print(F("Kd: "));
  display.println(kd);

    display.display();
}

void loop() {
  // Wait for a BLE central to connect
  BLEDevice central = BLE.central();

  ///////////////////
  // BALANCE ROBOT //
  ///////////////////
  float sampleSec = (millis() - currMillis) / 1000.0f;
  currMillis = millis();
  float angle = getAngle( & pid, sampleSec);
  balance( & pid, angle, sampleSec, dfw, dbw, dlf, drt);
  //Serial.println(angle);
  //processSerialInput( & pid);

  //lights and OLED
  //checkLights();
  #if 0
  float d_angle = getDesiredAngle( & pid);

  display.clearDisplay();

  display.setTextSize(1); // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0); // Start at top-left corner
  display.print(F("Desired Angle: "));
  display.println(d_angle);

  display.display();
  #endif

  #if 1
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate connection

    // Keep running while connected
    while (central.connected()) {
      //oLedLoop();
      checkLights();

      ///////////////////
      // BALANCE ROBOT //
      ///////////////////
      float sampleSec = (millis() - currMillis) / 1000.0f;
      currMillis = millis();
      float angle = getAngle( & pid, sampleSec);
      balance( & pid, angle, sampleSec, dfw, dbw, dlf, drt);
      //Serial.println(angle);
      //processSerialInput( & pid);

      // Check if the characteristic was written
      if (customCharacteristic.written()) {
        //oLedLoop();
        // Get the length of the received data
        int length = customCharacteristic.valueLength();

        // Read the received data
        const unsigned char * receivedData = customCharacteristic.value();

        // Create a properly terminated string
        char receivedString[length + 1]; // +1 for null terminator
        memcpy(receivedString, receivedData, length);
        receivedString[length] = '\0'; // Null-terminate the string

        // Print the received data to the Serial Monitor
        Serial.print("Received data: ");
        Serial.println(receivedString);

        // Split the received string by the comma

        //LEFT/RIGHT/HAZARD SIGNALS
        if (strcmp(receivedString, "LEFT SIGNAL") == 0) {
          flagL = 1;
        } else if (strcmp(receivedString, "RIGHT SIGNAL") == 0) {
          flagR = 1;
        } else if (strcmp(receivedString, "HAZARD") == 0) {
          flagL = 1;
          flagR = 1;
        } else if (strcmp(receivedString, "STOP") == 0) {
          sigOff();
        } else if (strcmp(receivedString, "FORWARD") == 0) {
          dfw = true;
          dbw = false;
          dlf = false;
          drt = false;
        } else if (strcmp(receivedString, "BACKWARDS") == 0) {
          dfw = false;
          dbw = true;
          dlf = false;
          drt = false;
        } else if (strcmp(receivedString, "LEFT") == 0) {
          dfw = false;
          dbw = false;
          dlf = true;
          drt = false;
        } else if (strcmp(receivedString, "RIGHT") == 0) {
          dfw = false;
          dbw = false;
          dlf = false;
          drt = true;
        } else {
          dfw = false;
          dbw = false;
          dlf = false;
          drt = false;
        }

        //PLATFROM EXTENSION
        actuatorLoop(receivedString);
        //AUDIO PLAYBACK

        play_music(receivedString);
        //UPDATE ANGLE
        // updateDesiredAngleCommmand( & pid, receivedString);
        // Optionally, respond by updating the characteristic's value
        // customCharacteristic.writeValue("Data received");
      } else {
        ///////////////////
        // BALANCE ROBOT //
        ///////////////////
        float sampleSec = (millis() - currMillis) / 1000.0f;
        currMillis = millis();
        float angle = getAngle( & pid, sampleSec);
        balance( & pid, angle, sampleSec, dfw, dbw, dlf, drt);
      }
      actuatorLoop("");
      playMusicNonBlocking();
    }

    // digitalWrite(LED_BUILTIN, LOW); // Turn off LED when disconnected
    // Serial.println("Disconnected from central.");
  }
  #endif
}