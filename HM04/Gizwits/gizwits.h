/**
 * @file gizwits.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#ifndef GIZWITS_H
#define GIZWITS_H

/**
 * @brief Define the timer used by gizwits and htim2 in gizwits_product.c should be instead of htim_gizwits.
 * 
 */
#define htim_gizwits htim7 //< htim2 or htim2 is the default choice by gizwits
#define TIM_GIZWITS TIM7

/**
 * @brief Define the timer used by gizwits and huart2 in gizwits_product.c should be instead of huart_wifi.
 * 
 */
#define huart_wifi huart2 ///< huart2 is the default choice by gizwits
#define huart_debug huart4 /// huart1 is the default setting by
#define USART_WIFI USART2


void InitGizwits(void);


#endif


