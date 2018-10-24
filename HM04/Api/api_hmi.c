/**
 * @file api_sys.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-16
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#include "api_hmi.h"
#include "cmsis_os.h"
#include <stdint.h>



/**
 * @brief 
 * 
 * @param read_keys_func_array 
 * @param keys_detect_callback 
 */
void ReadKeys(fptrReadKey_t read_keys_func_array[],
              fptrKeysDetectionCallback_t keys_detect_callback  
              ){
    static uint8_t keys_hold_acc[KEYS_NUM] = {0, 0, 0}; ///< This array for storing the duration of all keys
    static key_state_t keys_state[KEYS_NUM] = {RELEASE, RELEASE, RELEASE}; ///< This array for storing the state of all keys
    /// start to read all keys
    for(uint8_t i=0; i<KEYS_NUM; i++){
        keys_state[i] = read_keys_func_array[i](); ///< get the state of each key
        /// when each key is pressed, accumulate the duration and call the callback
        if(keys_state[i] == HOLD){
            keys_hold_acc[i]++;
            keys_detect_callback(keys_state, keys_hold_acc);

        }
        else{
            /// call the callback before clear the duration to let callback know that is the releasing action just after hold action
            keys_detect_callback(keys_state, keys_hold_acc);
            keys_hold_acc[i] = 0;
        }
    }
}

// void ReadKeysPeriodically(fptrReadKey_t read_keys_func_array[],
//                           fptrKeysDetectionPeriod_t delay_func,
//                           uint8_t delays,
//                           fptrKeysDetectionCallback_t keys_detect_callback ){
    
//     while(1){
//         Readkeys(read_keys_func_array, keys_detect_callback);
//         delay_func(delays);
//     }
// }


key_state_t ReadKeyLamp(void){
    if (ReadKey(key_lamp) == GPIO_PIN_RESET) {
        return HOLD;
    }
    else 
        return RELEASE;
}

key_state_t ReadKeyMist(void){
    if (ReadKey(key_mist) == GPIO_PIN_RESET) {
        return HOLD;
    }
    else 
        return RELEASE;
}

key_state_t ReadKeyPair(void){
    if (ReadKey(key_pair) == GPIO_PIN_RESET) {
        return HOLD;
    }
    else 
        return RELEASE;
}

extern osTimerId PairingHmiTimerHandle;
void StartPairing_hmi(void){
    osTimerStart(PairingHmiTimerHandle, 100);
}

void StopPairing_hmi(void){

}