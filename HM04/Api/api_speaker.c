


#include "api_speaker.h"



const uint8_t Command_PlayPause[3] = {0x55, 0x06, 0x55};
const uint8_t Command_LastSong[3] = {0x55, 0x04, 0x55};
const uint8_t Command_NextSong[3] = {0x55, 0x05, 0x55};


speaker_t speaker;

uint8_t InitSpeaker(void){
    speaker.status = pause;
    speaker.mode = bt;
    
}

speaker_t *GetSpeaker(void){
    return &speaker;
}

uint8_t PlayPause(void){
    if (speaker.status == play){
        
        speaker.status = pause;
    }
    else if (speaker.status == pause){

        speaker.status = play;
    }
    else {}
}


