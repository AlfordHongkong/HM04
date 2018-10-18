/**
 * @file bsp_lamp.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-17
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#ifndef BSP_LAMP_H
#define BSP_LAMP_H

#include "stdint.h"

typedef enum{
    lamp_red,
    lamp_green,
    lamp_blue,
    lamp_white,
    lamp_yellow,
}which_lamp_t;

void InitLampPWM(void);
void SetLampPWM(which_lamp_t lamp, uint8_t pwm_pulse);
void TestLampPwm(void);

#endif

