/**
 * @file bsp_hm04.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "bsp_hm04.h"
#include "bsp_gpio.h"
#include "bsp_lamp.h"


void InitBSP(void){
    InitLampPWM();
    
}