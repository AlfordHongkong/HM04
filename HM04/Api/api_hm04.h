/**
 * @brief 
 * 
 * @file api_hm04.h
 * @author your name
 * @date 2018-09-11
 */


#ifndef API_HM04_H
#define API_HM04_H

#include "api_speaker.h"
#include "api_lamp.h"
#include "api_mist.h"

/**
 * @brief hm04 on or off
 * 
 */
typedef enum {
    hm04_on,
    hm04_off
}hm04_status_t;

/**
 * @brief struct for water tank
 * 
 */
typedef struct {
    uint8_t flag_water_shortage : 1;
    uint8_t remaining_water_percent : 7;
}water_tank_t;

/**
 * @brief type for hm04
 * 
 */
typedef struct {
    hm04_status_t status;
    lamp_t * lamp;
    mist_t * mist;
    speaker_t * speaker;
    water_tank_t * water_tank;
    
}hm04_t;

/**
 * @brief Initialize the whole device
 * 
 * @return uint8_t 
 */
uint8_t InitHM04(void);

uint8_t TurnOnHM04(void);
uint8_t TurnOffHM04(void);




#endif