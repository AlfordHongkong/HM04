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
    mist_off,
    mist_on,
    
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
    uint8_t flagWaterDeficient;
}mist_t;

#define TURN_OFF_MISTING_DELAY 10000 ///< when turn off misting, turn off mist immediately and turn off fan 10 seconds late.
#define MISTING_INTERMITTENT_DURATION 15000

/*=================================================================
                    Function Declaring
=================================================================*/
/**
 * @brief 
 *      Initialze all data relating to controling of mist component.
 * 
 */
void InitMist(void);


void MistingIntermittentCallback_api_mist(void);
void MistingTimerCallback_api_mist(void);
void StoppingMistingDelayCallback_api_mist(void);

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


#define TurnOffMistingDelayCallback_api_mist() TurnOffFan()
// uint8_t SwitchMistTimer(mist_timer_t timer);

/**
 * @brief switch the mode of misting
 * 
 * @param mode - mode to be set
 * @return uint8_t - 0 means return success
 */
uint8_t SwitchMistMode(mist_mode_t mode);

/**
 * @brief check whether misting stated or not
 * 
 * @return uint8_t 
 */
uint8_t IsMistStarted(void);


/**
 * @brief Get the Mist 
 * 
 * @return mist_t* 
 */
mist_t * GetMist(void);

/**
 * @brief Get the Mist Status 
 * 
 * @return mist_status_t 
 */
mist_status_t GetMistStatus(void);


/**
 * @brief Get the Mist Mode 
 * 
 * @return mist_mode_t 
 */
mist_mode_t GetMistMode(void);

/**
 * @brief action for the key mist pressed
 * 
 * @return uint8_t - 0 means return success
 */
uint8_t Do4KeyMistPressed(void);

#endif