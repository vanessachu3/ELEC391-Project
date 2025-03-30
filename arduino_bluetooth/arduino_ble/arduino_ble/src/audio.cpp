#include <audio.h>



void audioSetup(){
        Serial.begin(115200);
        pinMode(speaker_pin, OUTPUT);
        
    }

// void play_music(const char* command) {
//     while(strcmp(command, "MUSIC_ON") == 0){
//         for (int i = 0; i < sizeof(ice_cream_song)/sizeof(ice_cream_song[0]); i++) {
//             tone(speaker_pin, ice_cream_song[i], 1000/note_duration[i]);
//             delay(1000/note_duration[i] * 1.30);
//         };
//     }
//     //stop music
//      if (strcmp(command, "MUSIC _OFF") == 0){
//         noTone(speaker_pin);
//     } 
// }
void kill_music(){
    noTone(speaker_pin);
}
