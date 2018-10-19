/**
 * @file api_mist.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#include "api_mist.h"
#include "bsp_gpio.h"

/**
 * @brief 
 *      Declare and define the mist data struct
 * 
 */
static mist_t mist;

/**
 * @brief 
 *      Initialze all data relating to controling of mist component.
 * 
 */
void InitMist(void){
    /// Initialize the mist data
    mist.mode = continuous;
    mist.timer = no_timer;
    mist.status = mist_off;
    mist.isWaterDeficient = 0;

}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t TurnOnMistAndFan(void){
    TurnOnFan();
    TurnOnMist();

    return 1;
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t TurnOffMistAndFan(void){
    TurnOffMist();
    /// delay 10s to let the mist dissipating.

    TurnOffFan();

    return 1;
}

uint8_t StartMisting(void){
    /// checking if lacking water
    if(mist.isWaterDeficient == 1)
        return 0;
    /// checking what mist_mode is

    /// checking what mist_timer is 

    /// change the setting
    mist.status = mist_on;
    /// then do action
    TurnOnMistAndFan();

    return 1;
}

uint8_t StopMisting(void){
    /// change the setting
    mist.status = mist_off;
    /// then, do action
    TurnOffMistAndFan();

    return 1;
}

uint8_t SetMistMode(mist_mode_t mode){
    /// first, change the setting
    mist.mode = mode;
    /// then actions

    
    return 1;
}

uint8_t SetMistTimer(mist_timer_t timer){
    /// first, change the setting
    mist.timer = timer;
    /// then actions

    return 1;
}


mist_t * GetMist(void){
    return &mist;
}

uint8_t isMistStarted(void){
    if(mist.status == mist_on){
        return 1;
    }
    else{
        return 0;
    }
}
