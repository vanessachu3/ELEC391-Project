#include <audio.h>



void audioSetup(){
        Serial.begin(115200);
        pinMode(speaker_pin, OUTPUT);
        noTone(speaker_pin);
    }


void kill_music(){
    noTone(speaker_pin);
}
