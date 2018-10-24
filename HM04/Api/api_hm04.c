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

static hm04_t hm04;

uint8_t InitHM04(void){
    /// first, initialize the bsp
    InitBSP();
    /// second, initialize api of each component
    InitLamp();
    InitMist();

    /// initialize hm04
    hm04.lamp = GetLamp();
    hm04.mist = GetMist();
    hm04.status = hm04_off;

    return 1;
}

uint8_t SetHm04Status(hm04_status_t status){
    hm04.status = status;

    return 1;
}


uint8_t TurnOnHM04(void){

    return 1;
}


uint8_t TurnOffHM04(void){
    
    return 1;
}

uint8_t IsHm04TurnedOn(void){
    if (hm04.status == hm04_on)
        return 1;
    
    return 0;
}