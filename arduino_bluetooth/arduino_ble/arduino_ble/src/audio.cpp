#include <audio.h>
#define speaker_pin A7 //TO BE CHANGED

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
void audioSetup(){
        Serial.begin(115200);
        pinMode(speaker_pin, OUTPUT);
    }
void play_music(const char* command) {
    if(strcmp(command, "MUSIC ON") == 0){
        for (int i = 0; i < sizeof(ice_cream_song)/sizeof(ice_cream_song[0]); i++) {
            tone(speaker_pin, ice_cream_song[i], 1000/note_duration[i]);
            delay(1000/note_duration[i] * 1.30);
        };
    }
    //stop music
    else if (strcmp(command, "MUSIC OFF") == 0){
        return;
    }
    
}
