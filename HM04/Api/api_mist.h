/**
 * @brief 
 * 
 * @file api_mist.h
 * @author your name
 * @date 2018-09-11
 */

#ifndef API_MIST_H
#define API_MIST_H

#include <stdint.h>

/**
 * @brief mist status
 * 
 */
typedef enum {
    mist_on,
    mist_off
}mist_status_t;

/**
 * @brief mist modes
 * 
 */
typedef enum {
    continuous,
    intermittent,
    mist_mode_max
}mist_mode_t;

/**
 * @brief 
 * 
 */
typedef enum {
    timer_60min,
    timer_120min,
    no_timer
}mist_timer_t;

/**
 * @brief mist_t structure
 * 
 */
typedef struct {
    mist_status_t status;
    mist_mode_t mode;
    mist_timer_t timer;
}mist_t;

mist_t * GetMistStruct(void);
uint8_t TurnMistOn(void);
uint8_t TurnMistOff(void);
uint8_t ChangeMistMode(mist_mode_t mode);

#endif