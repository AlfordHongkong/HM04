/**
 * @file api_hm04.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#include "api_hm04.h"
#include "bsp_hm04.h"

uint8_t InitHM04(void){
    /// first, initialize the bsp
    InitBSP();
    /// second, initialize api of each component
    InitLamp();
    InitMist();

    return 1;
}


uint8_t TurnOnHM04(void){

    return 1;
}


uint8_t TurnOffHM04(void){
    
    return 1;
}
