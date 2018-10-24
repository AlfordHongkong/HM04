/**
 * @file bsp_lamp.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-17
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "bsp_lamp.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

#define htim_lamp_rgb htim3
#define TIM_CHANNEL_RED TIM_CHANNEL_2
#define TIM_CHANNEL_GREEN TIM_CHANNEL_3
#define TIM_CHANNEL_BLUE TIM_CHANNEL_1

#define htim_lamp_wy htim2
#define TIM_CHANNEL_WHITE TIM_CHANNEL_4
#define TIM_CHANNEL_YELLOW TIM_CHANNEL_3

/**
 * @brief start the PWM output
 * 
 */
void InitLampPWM(void){
    HAL_TIM_PWM_Start(&htim_lamp_rgb, TIM_CHANNEL_RED);
    HAL_TIM_PWM_Start(&htim_lamp_rgb, TIM_CHANNEL_GREEN);
    HAL_TIM_PWM_Start(&htim_lamp_rgb, TIM_CHANNEL_BLUE);
    HAL_TIM_PWM_Start(&htim_lamp_wy, TIM_CHANNEL_WHITE);
    HAL_TIM_PWM_Start(&htim_lamp_wy, TIM_CHANNEL_YELLOW);
}

/**
 * @brief Set the Lamp PWM object
 * 
 * @param lamp : defien which lamp to be set
 * @param pwm_pulse : define the pulse to be set
 */
void SetLampPWM(which_lamp_t lamp, uint8_t pwm_pulse){
    TIM_OC_InitTypeDef sConfigOC;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.Pulse = pwm_pulse;

    switch (lamp){
        case lamp_red:
        HAL_TIM_PWM_ConfigChannel(&htim_lamp_rgb, &sConfigOC, TIM_CHANNEL_RED);
        HAL_TIM_PWM_Start(&htim_lamp_rgb, TIM_CHANNEL_RED);
        break;

        case lamp_green:
        HAL_TIM_PWM_ConfigChannel(&htim_lamp_rgb, &sConfigOC, TIM_CHANNEL_GREEN);
        HAL_TIM_PWM_Start(&htim_lamp_rgb, TIM_CHANNEL_GREEN);
        break;

        case lamp_blue:
        HAL_TIM_PWM_ConfigChannel(&htim_lamp_rgb, &sConfigOC, TIM_CHANNEL_BLUE);
        HAL_TIM_PWM_Start(&htim_lamp_rgb, TIM_CHANNEL_BLUE);
        break;

        case lamp_white:
        HAL_TIM_PWM_ConfigChannel(&htim_lamp_wy, &sConfigOC, TIM_CHANNEL_WHITE);
        HAL_TIM_PWM_Start(&htim_lamp_wy, TIM_CHANNEL_WHITE);
        break;

        case lamp_yellow:
        HAL_TIM_PWM_ConfigChannel(&htim_lamp_wy, &sConfigOC, TIM_CHANNEL_YELLOW);
        HAL_TIM_PWM_Start(&htim_lamp_wy, TIM_CHANNEL_YELLOW);
        break;

        default:
        break;
    }
}


void TestLampPwm(void){
    which_lamp_t lamp = lamp_red;
    while(lamp <= lamp_blue){
        for(int i=0; i<256; i++){
            SetLampPWM(lamp, i);
            HAL_Delay(10);
        }
        SetLampPWM(lamp, 0);
        lamp++;
    }
}
