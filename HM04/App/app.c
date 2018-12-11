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
#include "sw_iic.h"
#include "fsm.h"
#include "cmsis_os.h"
#include "gizwits_protocol.h"
#include <stdint.h>


extern osMessageQId eventsQueueHandle;



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
    
    event_t e;
    ///
    for(uint8_t i=0; i<KEYS_NUM; i++){
        switch(i){
            case KEY_LAMP_ORDER:
            if(keys_state[i] == HOLD){

            }
            else{
                if(keys_hold_acc[i]>0 && keys_hold_acc[i]<10){
                    /// it's a click
                    e = EVENT_LAMP_KEY_SHORT;
                    xQueueSend(eventsQueueHandle, &e, 10);                }
            }
            break;

            case KEY_MIST_ORDER:
            if(keys_state[i] == HOLD){

            }
            else{
                if(keys_hold_acc[i]>0 && keys_hold_acc[i]<10){
                    e = EVENT_MIST_KEY_SHORT;
                    xQueueSend(eventsQueueHandle, &e, 10);
                }
            }
            break;

            case KEY_PAIR_ORDER:
            /// when the key is being hold.
            if(keys_state[i] == HOLD){
            	if(keys_hold_acc[i] == 40){
                    e = EVENT_PAIR_KEY_LONG;
                    xQueueSend(eventsQueueHandle, &e, 10);
                }
            }
            else{
                if(keys_hold_acc[i]>0 && keys_hold_acc[i]<10){
                    e = EVENT_PAIR_KEY_SHORT;
                    xQueueSend(eventsQueueHandle, &e, 10);
                }
            }
            break;

            default:
            break;
        }
    }
}


void SystemStartSignal(void){
    SetLampPWM(lamp_red, 100);
    HAL_Delay(300);
    SetLampPWM(lamp_red, 0);
    SetLampPWM(lamp_green, 100);
    HAL_Delay(300);
    SetLampPWM(lamp_green, 0);
    SetLampPWM(lamp_blue, 100);
    HAL_Delay(300);
    SetLampPWM(lamp_blue, 0);
    SetLampPWM(lamp_yellow, 100);
    HAL_Delay(300);
    SetLampPWM(lamp_yellow, 0);
    SetLampPWM(lamp_white, 100);
    HAL_Delay(300);
    SetLampPWM(lamp_white, 0);
}





