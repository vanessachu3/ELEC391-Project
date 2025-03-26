#include <Arduino.h>
#include <pitches.h>

#define speaker_pin A7
unsigned int i = 0;
void play_music();

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

void setup() {
    Serial.begin(115200);
    pinMode(speaker_pin, OUTPUT);
}

void loop() {
    //Serial.println("penis");
    play_music();
    //delay(15000);
    Serial.println("hi");
}

void play_music() {
    for (int i = 0; i < sizeof(ice_cream_song)/sizeof(ice_cream_song[0]); i++){
        //e.g. quarter note = 1000ms / 4, eighth note = 1000ms/8, etc.
        int note_length = 1000/note_duration[i];
        tone(speaker_pin, ice_cream_song[i], note_length);

        int pauseBetweenNotes = note_length * 1.30;
        delay(pauseBetweenNotes);

        //noTone(speaker_pin);
        Serial.println(i);
    }
}