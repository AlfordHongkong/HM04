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
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

lamp_t lamp;

/// for lamp dynamic
extern osTimerId LampDynamicTimerHandle;



#define DYNAMIC_BEGINNING_R  10
#define DYNAMIC_BEGINNING_G  100
#define DYNAMIC_BEGINNING_B  150


void InitLamp(void){
    /// first, initialize the bsp
    /// second, initialize the lamp data.
    lamp.status = lamp_off;
    lamp.mode = static_mode;
    lamp.scenario = morninig;
    lamp.static_color.r = 10;
    lamp.static_color.g = 100;
    lamp.static_color.b = 150;
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

lamp_status_t GetLampStatus(void){
    return lamp.status;
}

uint8_t IsLampTurnedOn(void){
	if(lamp.status == lamp_on)
		return 1;
	return 0;
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
    if(lamp.mode == white_mode){
        SetLampPWM(lamp_white, lamp.brightness_percent);
    }
    else if(lamp.mode == yellow_mode){
        SetLampPWM(lamp_yellow, lamp.brightness_percent);
    }
    else if(lamp.mode == static_mode){ ///< 
        SetLampPWM(lamp_red, lamp.static_color.r);
        SetLampPWM(lamp_green, lamp.static_color.g);
        SetLampPWM(lamp_blue, lamp.static_color.b);
    }
    else if(lamp.mode == dynamic_mode){
        StartDynamicMode();
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
    SetLampPWM(lamp_red, 0);  
    SetLampPWM(lamp_green, 0);
    SetLampPWM(lamp_blue, 0);
    /// turn off white and yellow lamps.
    SetLampPWM(lamp_white, 0);
    SetLampPWM(lamp_yellow, 0);

    /// stop dynamic mode
    if(lamp.mode == dynamic_mode)
        StopDynamicMode();

    /// 
    lamp.status = lamp_off;

    return 1;
}

uint8_t SwitchLampMode(lamp_mode_t mode){
    /// first, turn off all lam;
    TurnOffLamp();
    // switch(mode){
    //     case white_mode:
    //     SetLampPWM(lamp_white, lamp.brightness_percent);
    //     break;

    //     case yellow_mode:
    //     SetLampPWM(lamp_yellow, lamp.brightness_percent);
    //     break;

    //     case dynamic_mode:
    //     StartDynamicMode();
    //     break;

    //     case static_mode:
    //     SetLampPWM(lamp_red, lamp.static_color.r);
    //     SetLampPWM(lamp_green, lamp.static_color.r);
    //     SetLampPWM(lamp_blue, lamp.static_color.r);
    //     break;

    //     case scenario_mode:
    //     break;

    //     default:
    //     break;
    // }

    /// at least, change the setting
    lamp.mode = mode;
    TurnOnLamp();
    return 1;
}

uint8_t StartDynamicMode(void){
    lamp.static_color.r = DYNAMIC_BEGINNING_R;
    lamp.static_color.g = DYNAMIC_BEGINNING_G;
    lamp.static_color.b = DYNAMIC_BEGINNING_B;
    osTimerStart(LampDynamicTimerHandle, 100);
    return 1;
}

uint8_t StopDynamicMode(void){
    osTimerStop(LampDynamicTimerHandle);
    return 1;
}

void LampDynamicCallbackFromApiLamp(void){
    lamp.static_color.r++;
    lamp.static_color.g++;
    lamp.static_color.b++;
    SetLampPWM(lamp_red, lamp.static_color.r);
    SetLampPWM(lamp_green, lamp.static_color.g);
    SetLampPWM(lamp_blue, lamp.static_color.b);
}

void Do4KeyLampPressed(void){
    if(lamp.status == lamp_off){
        TurnOnLamp();
        SwitchLampMode(white_mode);
    }
    else {
		switch(lamp.mode){
			case white_mode:
			SwitchLampMode(yellow_mode);
			break;
			case yellow_mode:
			SwitchLampMode(dynamic_mode);
			break;
			case dynamic_mode:
			StopDynamicMode();
			SwitchLampMode(static_mode);
			break;
			case static_mode:
			TurnOffLamp();
			break;
			case scenario_mode:
			SwitchLampMode(white_mode);
			break;

			default:
			SwitchLampMode(white_mode);
			break;

		}
    }
}


color_group_t GetLampColor(void){
    return lamp.static_color;
}

uint8_t SetLampColor(color_group_t color){
    /// when the color is white and yellow
    lamp.static_color = color;

    /// when the color is not white and yellow

    return 1;
}



// uint8_t SetLampWhite(uint8_t brightness){
//     SetLampPWM(lamp_white, brightness);
//     return 1;
// }

// uint8_t SetLampYellow(uint8_t brightness){
//     SetLampPWM(lamp_yellow, brightness);

//     return 1;
// }

uint8_t SetScenario(scenario_t scenario){
    /// first, change the setting
    //lamp.last_scenario = scenario;
    /// then, do action


    return 1;
}


