/**
 * @file api_mist.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#include "api_mist.h"
#include "api_hm04.h"
#include "bsp_gpio.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"


extern osTimerId MistTimerHandle;
extern osTimerId MistIntermittentTimerHandle;
extern osTimerId TurnOffMistingDelayTimerHandle;


uint8_t UpdateMistLeds(void);


/**
 * @brief 
 *      Declare and define the mist data struct
 * 
 */
static mist_t mist;

/**
 * @brief 
 *      Initialze all data relating to controling of mist component.
 * 
 */
void InitMist(void){
    /// Initialize the mist data
    mist.mode = continuous;
    mist.timer = no_timer;
    mist.status = mist_off;
    mist.flagWaterDeficient = 0;

}

/*===============================================================================
                 software timers for application of misting
===============================================================================*/

#define StartStoppingMistingDelay() osTimerStart(TurnOffMistingDelayTimerHandle, TURN_OFF_MISTING_DELAY)
#define StopStoppingMistingDelay() osTimerStop(TurnOffMistingDelayTimerHandle)
// uint8_t StartStoppingMistingDelay(void){
//     osTimerStart(TurnOffMistingDelayTimerHandle, TURN_OFF_MISTING_DELAY);
// }
// uint8_t StopStoppingMistingDelay(void){
//     osTimerStop(TurnOffMistingDelayTimerHandle);
// }

osStatus StartMistTimer(uint8_t minutes){
    osStatus timerStatus;

    timerStatus = osTimerStart(MistTimerHandle, (minutes*1000*60));
    
    return timerStatus;
}

#define StopMistTimer()  osTimerStop(MistTimerHandle)
// uint8_t StopMistTimer(void){
//     osTimerStop(MistTimerHandle);
//     return 1;
// }

#define StartIntermittentMode() osTimerStart(MistIntermittentTimerHandle, MISTING_INTERMITTENT_DURATION)
#define StopIntermittentMode() osTimerStop(MistIntermittentTimerHandle)
// uint8_t StartIntermittentMode(void){
//     osTimerStart(MistIntermittentTimerHandle, MISTING_INTERMITTENT_DURATION);

//     return 1;
// }
// uint8_t StopIntermittentMode(void){
//     osTimerStop(MistIntermittentTimerHandle);

//     return 1;
// }

/***************
    callbacks
****************/
void MistingIntermittentCallback_api_mist(void){
    static uint8_t i = 0;
    if(i%2 == 0){
        /// start fan and mist
        TurnOnFan();
        TurnOnMist();
    }
    else{ /// stop fan and mist
        TurnOffFan();
        TurnOffMist();
    }
    i++;
}

void MistingTimerCallback_api_mist(void){
    StopMisting();
}

void StoppingMistingDelayCallback_api_mist(void){
    TurnOffFan();
}
/*===============================================================================
                            
===============================================================================*/

uint8_t StartMisting(void){
    osStatus timerStatus;
    /// checking if lacking water
    if(mist.flagWaterDeficient == 1)
        return 0;

    /// checking if the fan still not be turned off, after stop misting last time
    StopStoppingMistingDelay();
    if(timerStatus == osErrorResource){
        /// it means the timer is not started. 
    }
    /// checking what mist_mode is
    if(mist.mode == intermittent){
        StartIntermittentMode();
    }
    /// checking what mist_timer is 
    // switch(mist.timer){
    //     case no_timer:
    //     break;
    //     case timer_60min:
    //     StartMistTimer(60);
    //     break;
    //     case timer_120min:
    //     StartMistTimer(120);
    //     break;

    //     default:
    //     break;
    // }
    /// turn on fan and mist
    TurnOnFan();
    TurnOnMist();
    

    /// last, modify the flag data
    mist.status = mist_on;
    if(GetHm04Status() == hm04_off){
        SetHm04Status(hm04_on);
    }
    
    /// update indicator leds
    UpdateMistLeds();
    
    return 1;
}

uint8_t StopMisting(void){
    //osStatus timerStatus;
    if(mist.status == mist_on){
        /// checking mode
        if(mist.mode == intermittent){
            StopIntermittentMode();
        }
        /// checking timer
        // if(mist.timer != no_timer){
        //     StopMistTimer();
        // }
        /// stop fan and mist
        TurnOffMist();
        /// delay 10s to let the mist dissipating.
        StartStoppingMistingDelay();

        mist.status = mist_off;

        /// update indicator leds
        UpdateMistLeds();
    }

    return 1;
}

uint8_t SwitchMistMode(mist_mode_t mode){
    if(mist.status == mist_on){
        if(mode == mist.mode){
            /// nothing need to do.
            return 0;
        }
        else{
            /// switch the two modes
            if(mist.mode == continuous){
                StartIntermittentMode();
                mist.mode = intermittent;

                //return 1;
            }
            else if(mist.mode == intermittent){
                StopIntermittentMode();
                TurnOnFan();
                TurnOnMist();
                mist.mode = continuous;

                //return 1;
            }
            else{
                /// wrong mode
                return 0;
            }
            /// update indicator leds
            UpdateMistLeds();
        }
    }
    else{
        /// mist is turned off.
        return 0;
    }

    return 1;
}

uint8_t SwitchMistTimer(mist_timer_t timer){
    if(mist.status == mist_on){
        if(timer == mist.timer){
            /// nothing need to do
            return 0;
        }
        else{
            if(timer == timer_60min){
                StartMistTimer(60);
                mist.timer = timer_60min;

                //return 1;
            }
            else if(timer == timer_120min){
                StartMistTimer(120);
                mist.timer = timer_120min;

                //return 1;
            }
            else if(timer == no_timer){
                StopMistTimer();
                mist.timer = no_timer;

                //return 1;
            }
            else{
                /// wrong timer
                return 0;
            }
            /// update indicator leds
            UpdateMistLeds();
        }
    }
    else {
        /// mist is turned off
        return 0;
    }

    return 1;
}


uint8_t Do4KeyMistPressed(void){
    if(mist.status == mist_off){
        StartMisting();
        SwitchMistMode(continuous);
    }
    else if(mist.status == mist_on){
        if(mist.mode == continuous){
        	SwitchMistMode(intermittent);
        }
        else if(mist.mode == intermittent){
            StopMisting();
        }
        else{
            /// wrong mode
            return 0;
        }
    }
    else {
        /// wrong mist.status
        return 0;
    }

    return 1;
}

uint8_t UpdateMistLeds(void){
    if(mist.status == mist_off){
        // TurnOffLed(led_on);
        TurnOffLed(led_on);
        TurnOffLed(led_int);
    }
    else if(mist.status == mist_on){
        if (mist.mode == continuous){
            TurnOnLed(led_on);
            TurnOffLed(led_int);
        }
        else if (mist.mode == intermittent){
            TurnOffLed(led_on);
            TurnOnLed(led_int);
        }
        else {
            /// wrong timer
            return 0;
        }
    }
    else{
        /// wrong status
        return 0;
    }

    return 1;
}



mist_t * GetMist(void){
    return &mist;
}

uint8_t IsMistStarted(void){
    if(mist.status == mist_on){
        return 1;
    }
    else{
        return 0;
    }
}


mist_status_t GetMistStatus(void){
    return mist.status;
}

mist_mode_t GetMistMode(void){
    return mist.mode;
}