#include <Arduino.h>
#include <pitches.h>

#define speaker_pin A7
unsigned int i = 0;
void play_music();

int ice_cream_song[] = {
    NOTE_C4, NOTE_B3, 
    NOTE_A3, NOTE_A3, NOTE_B3, 
    NOTE_A3, NOTE_E3, NOTE_C3, NOTE_D3,
    NOTE_E3, NOTE_F3, NOTE_E3, NOTE_C3,
    NOTE_E3, NOTE_A3, NOTE_B3,
    NOTE_C4, NOTE_C4,
    NOTE_C4, NOTE_B3, NOTE_A3, NOTE_B3,
    NOTE_C4, NOTE_B3, 
    NOTE_B3, NOTE_C4, NOTE_B3,
    NOTE_A3, NOTE_A3, NOTE_B3,
    NOTE_A3, NOTE_E3, NOTE_C3, NOTE_D3,
    NOTE_E3, NOTE_F3, NOTE_E3, NOTE_C3,
    NOTE_E3, NOTE_A3, NOTE_B3,
    NOTE_C4, NOTE_E4, NOTE_E4, NOTE_F4,
    NOTE_E4, NOTE_C4, NOTE_A3, NOTE_B3,
    NOTE_C4, NOTE_B3, 
    NOTE_A3
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
    for (int i = 0; i < sizeof(ice_cream_song)/sizeof(ice_cream_song[0]) -1; i++){
        //e.g. quarter note = 1000ms / 4, eighth note = 1000ms/8, etc.
        int note_length = 1000/note_duration[i];
        tone(speaker_pin, ice_cream_song[i], note_length);

        int pauseBetweenNotes = note_length * 1.30;
        delay(pauseBetweenNotes);

        //noTone(speaker_pin);
        Serial.println(i);
    }
}