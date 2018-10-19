/**
 * @file api_lamp.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "api_lamp.h"

lamp_t lamp;

void InitLamp(void){
    /// first, initialize the bsp
    /// second, initialize the lamp data.
    lamp.status = lamp_off;
    lamp.mode = static_mode;
    lamp.last_scenario = morninig;
    lamp.last_static_color.r = 100;
    lamp.last_static_color.g = 100;
    lamp.last_static_color.b = 100;
    lamp.brightness_percent = 50;

}

/**
 * @brief Get the Lamp object
 * 
 * @return lamp_t* 
 */
lamp_t* GetLamp(void){
    return &lamp;
}

/**
 * @brief Set the Lamp Brightness object
 * 
 * @return uint8_t 
 */
uint8_t SetLampBrightness(uint8_t brightness){
    /// first, change the setting
    lamp.brightness_percent = brightness;
    /// then, do the action


    return 1;
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t TurnOnLamp(void){
    //// first, changet the setting
    lamp.status = lamp_on;
    /// then, actions
    /// checking the lamp mode
    if(lamp.mode == static_mode){ ///< 
        SetLampColor(lamp.last_static_color); ///< resume the last static color
    }
    else if(lamp.mode == dynamic_mode){

    }
    else if(lamp.mode == scenario_mode){

    }
    else{
        return 0;
    }

    return 1;
}

/**
 * @brief  Turn off all 5 lamps.
 * 
 * @return uint8_t 
 */
uint8_t TurnOffLamp(void){
    /// turn off r,g,b lamps

    /// turn off white and yellow lamps.
}
uint8_t SetLampMode(lamp_mode_t mode){
    /// first, change the setting
    lamp.mode = mode;
    /// then, do action


    return 1;
}
uint8_t SetLampColor(color_group_t color){
    /// when the color is white and yellow


    /// when the color is not white and yellow
    SetLampPWM(lamp_red, color.r);
    SetLampPWM(lamp_green, color.g);
    SetLampPWM(lamp_blue, color.b);

    return 1;
}
uint8_t SetScenario(scenario_t scenario){
    /// first, change the setting
    lamp.last_scenario = scenario;
    /// then, do action


    return 1;
}


