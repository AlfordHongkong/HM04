/**
 * @file bsp_gpio.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-18
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "stm32f1xx_hal.h"

/**
 * @brief x should be the user_label of corresponding GPIO setted in cubeMX.
 * 
 * @ex: HAL_GPIO_WritePin(led_wifi_GPIO_Port, led_wifi_Pin, GPIO_PIN_RESET); 
 */
#define TurnOnLed(x) HAL_GPIO_WritePin(x##_GPIO_Port, x##_Pin, GPIO_PIN_RESET)
#define TurnOffLed(x) HAL_GPIO_WritePin(x##_GPIO_Port, x##_Pin, GPIO_PIN_SET)
#define ToggleLed(x) HAL_GPIO_TogglePin(x##_GPIO_Port, x##_Pin)

#define TurnOnFan() HAL_GPIO_WritePin(fan_GPIO_Port, fan_Pin, GPIO_PIN_SET)
#define TurnOffFan() HAL_GPIO_WritePin(fan_GPIO_Port, fan_Pin, GPIO_PIN_RESET)

#define TurnOnMist() HAL_GPIO_WritePin(mist_GPIO_Port, mist_Pin, GPIO_PIN_SET)  
#define TurnOffMist() HAL_GPIO_WritePin(mist_GPIO_Port, mist_Pin, GPIO_PIN_RESET)


/**
 * @brief x should be the user_label of corresponding GPIO setted in cubeMX.
 * 
 * @ex: HAL_GPIO_ReadPin(key_pair_GPIO_Port, key_pair_Pin)
 */

#define ReadKey(x) HAL_GPIO_ReadPin(x##_GPIO_Port, x##_Pin)

#endif

