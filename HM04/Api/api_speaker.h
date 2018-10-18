/**
 * @brief 
 * 
 * @file api_speaker.h
 * @author your name
 * @date 2018-09-11
 */
#ifndef API_SPEAKER_H
#define API_SPEAKER_H

#include <stdint.h>

/**
 * @brief speaker status enum
 * 
 */
typedef enum {
    play,
    pause
} speaker_status_t;
/**
 * @brief structure for speaker handling
 * 
 */
//typedef struct speaker speaker_t;

/**
 * @brief 
 * 
 */
typedef struct {
    speaker_status_t status;
    uint8_t volum;
} speaker_t;


speaker_t GetSpeakerStruct(void);
uint8_t SetSpeakerStatus(speaker_status_t status);
uint8_t SetSpeakerVolume(uint8_t volume);
uint8_t FowardTheMusic(void);
uint8_t BackTheMusic(void);

#endif
