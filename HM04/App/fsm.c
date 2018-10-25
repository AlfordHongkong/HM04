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
        }
        else{  ///< when lamp is turned off
            if(new == EVENT_LAMP_KEY_SHORT){
                Do4KeyLampPressed();
            }
        }

        /// when mist is turned on
        if(IsMistStarted()){
            if(new == EVENT_MIST_KEY_SHORT){
                Do4KeyMistPressed();
            }
        }
        else{
            if(new == EVENT_MIST_KEY_SHORT){
                Do4KeyMistPressed();
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
