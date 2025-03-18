#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

#define SD_FAT_TYPE 3

// default CS pin for Adafruit BFF is TX
#define SD_CS_PIN D10
#define speaker_pin D9
// WAV file parameters (ensure your .wav file matches this)
#define SAMPLE_RATE 32000 // Hz
#define BUFFER_SIZE 256

File audioFile;
byte buffer[BUFFER_SIZE];

void setup() {
    Serial.begin(115200);
    // wait for serial port to open
    while (!Serial) {
      ;
    }

    // Initialize SD card
    Serial.println("Initializing SD card...");

    if (!SD.begin(SD_CS_PIN)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");

    // Open audio file
    audioFile = SD.open("ac.wav");
    if (!audioFile) {
        Serial.println("Failed to open file");
        return;
    }

    // Skip WAV header (first 44 bytes)
    audioFile.seek(44);

    // Set PWM frequency
    pinMode(speaker_pin, OUTPUT);

    // Increase PWM frequency (if supported by your board)
    //analogWriteFrequency(speaker_pin, 32000); // Some boards support this
}

void loop() {
  unsigned long startTime, elapsedTime;
  while (audioFile.available()) {
    int bytesRead = audioFile.read(buffer, BUFFER_SIZE);

    for (int i = 0; i < bytesRead; i++) {
        startTime = micros(); // Track when we start playing this sample

        analogWrite(speaker_pin, buffer[i]); // Output raw PCM data to PWM

        // Ensure proper timing (32kHz = 31.25 µs per sample)
        do {
            elapsedTime = micros() - startTime;
        } while (elapsedTime < 31);
    }
}

// Restart playback when the file ends
audioFile.seek(44);
}

