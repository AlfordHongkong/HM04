


#include "api_speaker.h"
#include "stm32f1xx_hal.h"




extern UART_HandleTypeDef huart1;
#define huart_bt huart1

#define COMMAND_LENGTH 8
const uint8_t Command_PlayPause[COMMAND_LENGTH] = {0X33, 0X3E, 0X00, 0X00, 0XA3, 0X00, 0X00, 0X00};
const uint8_t Command_LastSong[COMMAND_LENGTH] = {0X33, 0X3E, 0X00, 0X00, 0XA4, 0X00, 0X00, 0X00};
const uint8_t Command_NextSong[COMMAND_LENGTH] = {0X33, 0X3E, 0X00, 0X00, 0XA4, 0X01, 0X00, 0X00};
const uint8_t Command_VolumPlus[COMMAND_LENGTH] = {0X33, 0X3E, 0X00, 0X00, 0XA7, 0X00, 0X00, 0X00};
const uint8_t Command_VolumMinus[COMMAND_LENGTH] = {0X33, 0X3E, 0X00, 0X00, 0XA7, 0X01, 0X00, 0X00};
const uint8_t Command_ModeBT[COMMAND_LENGTH] = {0X33, 0X3E, 0X00, 0X00, 0XA1, 0X00, 0X00, 0X00};
const uint8_t Command_ModeLinein[COMMAND_LENGTH] = {0X33, 0X3E, 0X00, 0X00, 0XA1, 0X01, 0X00, 0X00};

speaker_t speaker;

uint8_t InitSpeaker(void){
    speaker.status = pause;
    speaker.mode = bt;
    
    SwitchToBT();
}

bt_mode_t GetSpeakerMode(void){
    return speaker.mode;
}


speaker_t *GetSpeaker(void){
    return &speaker;
}

uint8_t FowardTheMusic(void){
    HAL_UART_Transmit_IT(&huart_bt, Command_NextSong, COMMAND_LENGTH);

    return 0;
}
uint8_t BackTheMusic(void){
    HAL_UART_Transmit_IT(&huart_bt, Command_LastSong, COMMAND_LENGTH);

    return 0;
}
uint8_t VolumPlus(void){
    HAL_UART_Transmit(&huart_bt, Command_VolumPlus, COMMAND_LENGTH, 100);

    return 0;
}
uint8_t VolumMinus(void){
    HAL_UART_Transmit(&huart_bt, Command_VolumMinus, COMMAND_LENGTH, 100);

    return 0;
}
uint8_t SwitchToBT(void){
    HAL_UART_Transmit(&huart_bt, Command_ModeBT, COMMAND_LENGTH, 100);
    speaker.mode = bt;

    return 0;
}
uint8_t SwitchToLinein(void){
    HAL_UART_Transmit(&huart_bt, Command_ModeLinein, COMMAND_LENGTH, 100);
    speaker.mode = linein;

    return 0;
}
uint8_t PlayPause(void){
    HAL_UART_Transmit_IT(&huart_bt, Command_PlayPause, COMMAND_LENGTH);

    return 0;
}

uint8_t TestSpeaker(void){
    SwitchToLinein();
    for (int i=0; i<10; i++){
        VolumPlus();
        HAL_Delay(1000);
    }
    for(int i=0; i<5; i++){
        FowardTheMusic();
        HAL_Delay(3000);
    }
    for (int i=0; i<10; i++){
        VolumMinus();
        HAL_Delay(1000);
    }

}
