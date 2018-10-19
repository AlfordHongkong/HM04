/**
 * @file app.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-18
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "api_hmi.h"
#include "bsp_gpio.h"
#include "api_mist.h"
#include "api_lamp.h"
#include <stdint.h>



/**
 * @brief client/user of the api_hmi module defined the array of all keys for reading functions
 * 
 */
fptrReadKey_t ReadKeysFuncs[KEYS_NUM] = {
    ReadKeyLamp, 
    ReadKeyMist,
    ReadKeyPair
};

/**
 * @brief define the order of all keys
 * 
 */
#define KEY_LAMP_ORDER 0
#define KEY_MIST_ORDER 1
#define KEY_PAIR_ORDER 2

/**
 * @brief for getting the ReadKeysFuncs array
 * 
 * @return fptrReadKey_t* 
 */
fptrReadKey_t * GetReadKeysFuncs(void){
    return ReadKeysFuncs;
}

/**
 * @brief 
 * 
 * @param keys_state 
 * @param keys_hold_acc 
 */
void KeysCallback(key_state_t keys_state[], uint8_t keys_hold_acc[]){
    
    for(uint8_t i=0; i<KEYS_NUM; i++){
        switch(i){
            case KEY_LAMP_ORDER:
            if(keys_state[i] == HOLD){

            }
            else{
                if(keys_hold_acc[i]>0 && keys_hold_acc[i]<10){
                    /// it's a click
                	ToggleLed(led_1h);
                }
            }
            break;

            case KEY_MIST_ORDER:
            if(keys_state[i] == HOLD){

            }
            else{
                if(keys_hold_acc[i]>0 && keys_hold_acc[i]<10){
                    ///< it's a click
                	if(isMistStarted()){
                        StopMisting();
                    }
                    else{
                        StartMisting();
                    }
                }
            }
            break;

            case KEY_PAIR_ORDER:
            if(keys_state[i] == HOLD){
            	ToggleLed(led_on);
            }
            else{
                if(keys_hold_acc[i]>0 && keys_hold_acc[i]<10){

                    ToggleLed(led_wifi);
                }
            }
            break;

            default:
            break;
        }
    }
}





