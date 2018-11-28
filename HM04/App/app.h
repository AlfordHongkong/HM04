/**
 * @file app.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-18
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef APP_H
#define APP_H

#include "api_hmi.h"
#include <stdint.h>

/**
 * @brief for getting the ReadKeysFuncs array
 * 
 */
fptrReadKey_t * GetReadKeysFuncs(void);

/**
 * @brief 
 *      The callback of ReadKeys 
 * 
 * @param keys_state 
 * @param keys_hold_acc 
 */
void KeysCallback(key_state_t keys_state[], uint8_t keys_hold_acc[]);



#endif
