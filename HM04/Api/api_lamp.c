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
#include "api_hm04.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"




Scenario_t Scenario;
lamp_t lamp;
uint8_t flag_stop_scenario;
/// for lamp dynamic
extern osTimerId LampDynamicTimerHandle;


static uint8_t StartDynamicMode(void);
static uint8_t StopDynamicMode(void);
static uint8_t StartScenarioMode(void);
static uint8_t StopScenarioMode(void);
uint8_t ScenarioMorning(void);
uint8_t ScenarioDream(void);
uint8_t ScenarioNature(void);
uint8_t ScenarioOcean(void);
uint8_t ScenarioTropical(void);
uint8_t ScenarioRomantic(void);
static uint8_t ScenarioDelay(uint16_t one_ten_second);


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

    Scenario = ScenarioMorning;
    flag_stop_scenario = 1;

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

lamp_scenario_t GetLampScenario(void){
    return lamp.scenario;
}

uint8_t IsLampTurnedOn(void){
	if(lamp.status == lamp_on)
		return 1;
	return 0;
}

// /**
//  * @brief Set the Lamp Brightness object
//  * 
//  * @return uint8_t 
//  */
// uint8_t SetLampBrightness(uint8_t brightness){
//     uint8_t brightness_255;
//     brightness_255 = ((uint16_t)brightness * 255 / 100);
//     if(lamp.status == lamp_on){
//         if(lamp.mode == yellow_mode){
            
//             SetLampPWM(lamp_yellow, brightness_255);
//             lamp.brightness_percent = brightness;
//         }
//         else if(lamp.mode == white_mode){
//             SetLampPWM(lamp_white, brightness_255);
//             lamp.brightness_percent = brightness;
//         }
//         else{
//             /// wrong mode
//             return 1;
//         }
//     }
//     else{
//         /// lamp is not turned on
//         return 1;
//     }

//     return 0;
// }

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t TurnOnLamp(void){
    uint8_t brightness_255;
    brightness_255 = (uint16_t)lamp.brightness_percent * 255 / 100;
    /// first, actions
    /// checking the lamp mode
    if(lamp.mode == white_mode){
        SetLampPWM(lamp_white, brightness_255);
    }
    else if(lamp.mode == yellow_mode){
        SetLampPWM(lamp_yellow, brightness_255);
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
        StartScenarioMode();
    }
    else{
        return 1;
    }

    //// then, changet the setting
    lamp.status = lamp_on;
    if(GetHm04Status() == hm04_off){
        SetHm04Status(hm04_on);
    }

    return 0;
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

    if (lamp.mode == scenario_mode){
        StopScenarioMode();
    }
    /// 
    lamp.status = lamp_off;

    return 0;
}

uint8_t SwitchLampMode(lamp_mode_t mode){
    /// first, turn off all lam;
    TurnOffLamp();

    /// at least, change the setting
    lamp.mode = mode;
    TurnOnLamp();
    return 0;
}


#define DYNAMIC_MIN_R  50
#define DYNAMIC_MIN_G  0
#define DYNAMIC_MIN_B  0
#define DYNAMIC_MAX_R  150
#define DYNAMIC_MAX_G  100
#define DYNAMIC_MAX_B  100

typedef enum{
    r_go_up,
    g_go_down,
    b_go_up,

    r_go_down,
    g_go_up,
    b_go_down,
}dynamic_cation_t;
dynamic_cation_t dynamic_cation;

static uint8_t StartDynamicMode(void){
    lamp.static_color.r = DYNAMIC_MIN_R;
    lamp.static_color.g = DYNAMIC_MAX_G;
    lamp.static_color.b = DYNAMIC_MIN_B;
    dynamic_cation = r_go_up,
    osTimerStart(LampDynamicTimerHandle, 100);
    return 0;
}

static uint8_t StopDynamicMode(void){
    osTimerStop(LampDynamicTimerHandle);
    return 0;
}


void LampDynamicCallbackFromApiLamp(void){
    switch(dynamic_cation){
        case r_go_up:
        if(lamp.static_color.r++ >= DYNAMIC_MAX_R){
            dynamic_cation = g_go_down;
        }
        break;
        case g_go_down:
        if(lamp.static_color.g-- <= DYNAMIC_MIN_G+1){
            dynamic_cation = b_go_up;
        }
        break;
        case b_go_up:
        if(lamp.static_color.b++ >= DYNAMIC_MAX_B){
            dynamic_cation = r_go_down;
        }
        break;
        case r_go_down:
        if(lamp.static_color.r-- <= DYNAMIC_MIN_R+1){
            dynamic_cation = g_go_up;
        }
        break;
        case g_go_up:
        if(lamp.static_color.g++ >= DYNAMIC_MAX_G){
            dynamic_cation = b_go_down;
        }
        break;
        case b_go_down:
        if(lamp.static_color.b-- <= DYNAMIC_MIN_B+1){
            dynamic_cation = r_go_up;
        }
        break;

        default:
        break;
    }
    
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

lamp_mode_t GetLampMode(void){
    return lamp.mode;
}

uint8_t GetLampBrightness(void){
    return lamp.brightness_percent;
}

uint8_t SetLampColor(color_group_t color){
    /// when the color is white and yellow
    lamp.static_color = color;

    /// when the color is not white and yellow

    return 1;
}



uint8_t SetLampWhite(uint8_t brightness){
    lamp.brightness_percent = brightness;
    SwitchLampMode(white_mode);
    return 1;
}

uint8_t SetLampYellow(uint8_t brightness){
    lamp.brightness_percent = brightness;
    SwitchLampMode(yellow_mode);
    return 1;
}

// uint8_t SetScenario(scenario_t scenario){
//     /// first, change the setting
//     //lamp.last_scenario = scenario;
//     /// then, do action


//     return 1;
// }

/**********************************************
 *                 scenario fucntions
**********************************************/

uint8_t SwitchLampScenario(lamp_scenario_t scenario){
    
    TurnOffLamp();

    /// at least, change the setting
    lamp.scenario = scenario;
    switch (scenario){
        case morninig:
        Scenario = ScenarioMorning;
        break;
        case dream:
        Scenario = ScenarioDream;
        break;
        case romantic:
        Scenario = ScenarioRomantic;
        break;
        case ocean:
        Scenario = ScenarioOcean;
        break;
        case nature:
        Scenario = ScenarioNature;
        break;
        case tropical:
        Scenario = ScenarioTropical;
        break;

        default:
        break;
    }
    TurnOnLamp();
    return 0;
}


#define SCENARIO_DEBUG

#define SCENARIO_MORING_TIME_DELTA    3600
#define SCENARIO_DREAM_TIME_DELTA     9000
#define SCENARIO_ROMANTIC_TIME_DELTA  1500
#define SCENARIO_OCEAN_TIME_DELTA     2250
#define SCENARIO_NATURE_TIME_DELTA    3000
#define SCENARIO_TROPICAL_TIME_DELTA  2000

#define SCENARIO_SPEED_TIMERS 30
#define SCENARIO_DELAY_RUSOLUTION 10

uint8_t ScenarioMorning(void){
    uint8_t r = 255;
    uint8_t g = 25;
    uint8_t b = 0;
    for (uint8_t i = 0; i<250; i++){
        if (i < 205){
            g++;
        }
        else {
            b++;
        }
        SetLampPWM(lamp_red, r);
        SetLampPWM(lamp_green, g);
        SetLampPWM(lamp_blue, b);
        if (ScenarioDelay(SCENARIO_MORING_TIME_DELTA  / SCENARIO_SPEED_TIMERS / SCENARIO_DELAY_RUSOLUTION))
            return 1;
    }
    #ifdef SCENARIO_DEBUG
    printf("\n Morning done.\n");
    #endif
    return 0;
}

uint8_t ScenarioDream(void){
    uint8_t y = 100;
    uint8_t y255;
    for (; y>0; y--){
        y255 = (uint16_t)y * 255 / 100;
        SetLampPWM(lamp_yellow, y255);
        if (ScenarioDelay(SCENARIO_DREAM_TIME_DELTA  / SCENARIO_SPEED_TIMERS / SCENARIO_DELAY_RUSOLUTION))
            return 1;
    }
    SetLampPWM(lamp_yellow, 0);

    #ifdef SCENARIO_DEBUG
    printf("\n Dream done.\n");
    #endif

    return 0;
}

uint8_t ScenarioRomantic(void){
    uint8_t r = 255;
    uint8_t g = 0;
    uint8_t b = 220;
    for (uint16_t i=0; i<600; i++){
        if (i<220){
            b--;
        }
        else if (i<300){
            g++;
        }
        else if (i<380){
            g--;
        }
        else {
            b++;
        }
        SetLampPWM(lamp_red, r);
        SetLampPWM(lamp_green, g);
        SetLampPWM(lamp_blue, b);
        if (ScenarioDelay(SCENARIO_ROMANTIC_TIME_DELTA  / SCENARIO_SPEED_TIMERS / SCENARIO_DELAY_RUSOLUTION))
            return 1;
    }

    #ifdef SCENARIO_DEBUG
    printf("\n Romantic done.\n");
    #endif
    return 0;
}

uint8_t ScenarioOcean(void){
    uint8_t r =0;
    uint8_t g = 125;
    uint8_t b = 50;

    for (uint16_t i=0; i<400; i++){
        if (i < 200){
            b++;
        }
        else {
            b--;
        }

        SetLampPWM(lamp_red, r);
        SetLampPWM(lamp_green, g);
        SetLampPWM(lamp_blue, b);
        if (ScenarioDelay(SCENARIO_OCEAN_TIME_DELTA  / SCENARIO_SPEED_TIMERS / SCENARIO_DELAY_RUSOLUTION))
            return 1;
    }

    #ifdef SCENARIO_DEBUG
    printf("\n Ocean done.\n");
    #endif
    return 0;
}

uint8_t ScenarioNature(void){
    uint8_t r =0;
    uint8_t g = 255;
    uint8_t b = 20;

    for (uint16_t i=0; i<300; i++){
        if (i < 150){
            r++;
        }
        else {
            r--;
        }

        SetLampPWM(lamp_red, r);
        SetLampPWM(lamp_green, g);
        SetLampPWM(lamp_blue, b);
        if (ScenarioDelay(SCENARIO_NATURE_TIME_DELTA  / SCENARIO_SPEED_TIMERS / SCENARIO_DELAY_RUSOLUTION))
            return 1;
    }

    #ifdef SCENARIO_DEBUG
    printf("\n Nature done.\n");
    #endif
    return 0;
}

uint8_t ScenarioTropical(void){
    uint8_t r = 255;
    uint8_t g = 40;
    uint8_t b = 0;

    for (uint16_t i=0; i<450; i++){
        if (i < 215){
            r--;
            g++;
        }
        else if (i < 430) {
            r++;
            g--;
        }
        else{   
            g--;
        }

        SetLampPWM(lamp_red, r);
        SetLampPWM(lamp_green, g);
        SetLampPWM(lamp_blue, b);
        if (ScenarioDelay(SCENARIO_TROPICAL_TIME_DELTA  / SCENARIO_SPEED_TIMERS / SCENARIO_DELAY_RUSOLUTION))
            return 1;
    }

    #ifdef SCENARIO_DEBUG
    printf("\n Tropical done.\n");
    #endif
    return 0;
}

static uint8_t ScenarioDelay(uint16_t one_ten_second){
    for (uint16_t i=0; i<one_ten_second; i++){
        osDelay(SCENARIO_DELAY_RUSOLUTION);
        if (flag_stop_scenario){
            return 1;
        }
    }

    return 0;
}

extern osThreadId ScenarioTaskHandle;
static uint8_t StartScenarioMode(void){
    if (flag_stop_scenario ==1){
        /// need wating thread scenario to be suspended.
        while (osThreadSuspended != osThreadGetState(ScenarioTaskHandle)){
            
        }
    }
    osThreadResume(ScenarioTaskHandle);
    flag_stop_scenario = 0;
    
    return 0;
}
static uint8_t StopScenarioMode(void){
    // osThreadSuspend(ScenarioTaskHandle);

    flag_stop_scenario = 1;
    return 0;
}
