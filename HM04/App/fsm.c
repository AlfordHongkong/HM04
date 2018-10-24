/**
 * @file fsm.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-22
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#include "fsm.h"
#include "stm32f1xx_hal.h"
#include "api_hm04.h"
#include "api_hmi.h"
#include "cmsis_os.h"
#include "gizwits_protocol.h"


extern osMessageQId eventsQueueHandle;

void StartFSM(void){
    event_t new;
    if(xQueueReceive(eventsQueueHandle, &new, 10)){


        /// when hm04 is turend on
        // if(IsHm04TurnedOn()){

        // }
        // else{

        // }

        /// when the lamp is turned on
        if(IsLampTurnedOn()){
            if(new == EVENT_LAMP_KEY_SHORT){
                Do4KeyLampPressed();
            }
            // switch(GetLampStatus()){
            //     /// when the lamp is at white mode
            //     case white_mode:
            //     if(new == EVENT_LAMP_KEY_SHORT){
            //         SetLampMode(yellow_mode);
            //     }
            //     else{}
            //     break;

            //     /// when the lamp is at yellow mode
            //     case yellow_mode:
            //     if(new == EVENT_LAMP_KEY_SHORT){
            //         SetLampMode(dynamic_mode);
            //     }
            //     else{}
            //     break;

            //     /// when the lmap is at dynamic mode.
            //     case dynamic_mode:
            //     if(new == EVENT_LAMP_KEY_SHORT){
            //         //SetLampMode(yellow_mode);
            //         StopLampDynamic();
            //     }
            //     else{}
            //     break;

            //     /// when the lamp is at static mode.
            //     case static_mode:
            //     if(new == EVENT_LAMP_KEY_SHORT){
            //         TurnOffLamp();
            //     }
            //     else{}
            //     break;
            //     case scenario_mode:
            //     break;

            //     default:
            //     break;
            // }
        }
        else{  ///< when lamp is turned off
            if(new == EVENT_LAMP_KEY_SHORT){
                Do4KeyLampPressed();
            }
        }

        /// whether hm04 is turned on or not.
        // if (new == EVENT_START_PAIRING){
        //     /// start pairing
        //     gizwitsSetMode(WIFI_AIRLINK_MODE);
        //     /// show the pair hmi
        //     StartPairing_hmi();
        // }

        // if  (new == EVENT_STOP_PAIRING){
        //     /// stop pair hmi 
        // }
    }
}
