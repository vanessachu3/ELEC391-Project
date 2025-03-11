#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SPI.h>
#include <SD.h>
//#include "SdFat.h"
//SdFat SD;

#define SD_FAT_TYPE 3

// default CS pin for Adafruit BFF is TX
#define SD_CS_PIN D10

void setup() {
  Serial.begin(115200);
  // wait for serial port to open
  while (!Serial) {
    ;
  }

  Serial.println("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    myFile.println("hello bff! if this opens successfully then it writes successfully");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
    // remove the file;
    SD.remove("test.txt");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
