/**
 * @file api_hmi.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-18
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef API_HMI_H
#define API_HMI_H

#include "stm32f1xx_hal.h"
#include "bsp_gpio.h"


/**
 * @brief indicate whether the key is released or pressed.
 * 
 */
typedef enum{
    HOLD,
    RELEASE,
} key_state_t;

/**
 * @brief 
 *      THis is the functione pointer for gettin each state of keys
 * 
 */
typedef key_state_t (*fptrReadKey_t) (void);  

/**
 * @brief 
 * 
 */
//typedef void (*fptrKeysDetectionPeriod_t)(uint8_t x);

/**
 * @brief 
 *      This kinda callback is putted in the ReadKeys function and the callback will get the state and hold duration of all keys
 *      The parameter keys_state_array will store the states of all keys and passed by the caller.
 *      The parameter keys_hold_acc_array will store the pressed duration of all keys and passed by the caller.
 */
typedef void (*fptrKeysDetectionCallback_t)(key_state_t * keys_state_array, uint8_t * kays_hold_acc_array);


/**
 * @brief This macro is very important and client should define it manually.
 * 
 */
#define KEYS_NUM 3  ///< indicate how many keys in your system.

key_state_t ReadKeyLamp(void);  ///< key lamp detection function
key_state_t ReadKeyMist(void);
key_state_t ReadKeyPair(void);



/**
 * @brief 
 *      This function should be recalled periodically. For example, client could put it into a task of rtos or interrupt of a timer, 
 *  make it be recalled every 20ms or 50ms which defined by client. Client should define the read_keys_func_array, and this func will 
 *  pass the state and pressed duration of all keys to the callback. So client could use the callback to get the activities of all keys 
 *  and define the reponse of each activity.
 * 
 * 
 * @param read_keys_func_array 
 *      The point of the arrary storing state-reading functions of all keys
 * @param keys_detect_callback 
 *      The callback 
 */
void ReadKeys(fptrReadKey_t read_keys_func_array[],
              fptrKeysDetectionCallback_t keys_detect_callback);




/*======================================================================
                    Indicators / Leds
======================================================================*/
/**
 * @brief 
 *      Just refer the api from bsp. 
 *      ex:  TurnOnled(x); TurnOffled(x);
 *      x could be led_wifi, led_on, led_1h, led_2h
 * 
 */

void StartPairing_hmi(void);
void StopPairing_hmi(void);


#endif




