/**
 * @brief 
 *      This api is for controling both of the mist and fan
 * @file api_mist.h
 * @author your name
 * @date 2018-09-11
 */

#ifndef API_MIST_H
#define API_MIST_H

#include <stdint.h>


/*=================================================================
                    Date types
=================================================================*/


/**
 * @brief mist status
 *      Indicate the mist component is working or not.
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
    uint8_t isWaterDeficient;
}mist_t;

/*=================================================================
                    Function Declaring
=================================================================*/
/**
 * @brief 
 *      Initialze all data relating to controling of mist component.
 * 
 */
void InitMist(void);

/**
 * @brief 
 *      Turn on the fan and mist immediately.
 * 
 * @return uint8_t 
 */
uint8_t TurnOnMistAndFan(void);

/**
 * @brief 
 *      Turn on mist immediately and turn off fan 10s later to let the remainning mist dissipate.
 * 
 * @return uint8_t 
 */
uint8_t TurnOffMistAndFan(void);

/**
 * @brief start misting.
 * 
 * @return uint8_t 
 */
uint8_t StartMisting(void);

/**
 * @brief stop misting
 * 
 */
uint8_t StopMisting(void);

uint8_t isMistStarted(void);


uint8_t SetMistMode(mist_mode_t mode);
uint8_t SetMistTimer(mist_timer_t timer);
mist_t * GetMist(void);

#endif