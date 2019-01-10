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
#include "api_mist.h"
#include "cmsis_os.h"
#include "gizwits_protocol.h"
#include "gizwits_product.h"
#include "api_speaker.h"


#define PRINT_FSM

static void FSM_LAMP_ON_REGULAR(lamp_mode_t mode, event_t event);

extern osMessageQId eventsQueueHandle;

void StartFSM(void){
    // lamp_t *lamp;
//    uint8_t brightness_percentage = 0;
    event_t new;
//    static uint8_t lamp_white_brightness_level = 0;
//    static uint8_t lamp_yellow_brightness_level = 0;
    
    if(xQueueReceive(eventsQueueHandle, &new, 10)){

        lamp_mode_t mode;
        mode = GetLampMode();
        /// when the lamp is turned on
        if(IsLampTurnedOn()){
            switch(mode){
                /// STATIC MODE
                case static_mode:
                    FSM_LAMP_ON_REGULAR(mode, new);
                break;
                /// DYNAMIC MODE
                case dynamic_mode:
                    FSM_LAMP_ON_REGULAR(mode, new);
                break;
                /// SCENARIO MODE
                case scenario_mode:
                    FSM_LAMP_ON_REGULAR(mode, new);
                break;
                /// WHITE MODE
                case white_mode:
                    FSM_LAMP_ON_REGULAR(mode, new);
                break;
                /// YELLOW MODE
                case yellow_mode:
                    FSM_LAMP_ON_REGULAR(mode, new);
                break;
                default:
                break;
            }
        }
        else{  ///< when lamp is turned off
            if(new == EVENT_LAMP_KEY_SHORT){
                Do4KeyLampPressed();
            }
            else if (new == EVENT_IR_LAMP){
                TurnOnLamp();
            }
            else if (new == EVENT_WIFI_LAMP_POWER_ON){
                TurnOnLamp();
            }
        }

        /// when mist is turned on
        if(IsMistStarted()){
            mist_mode_t mist_mode;
            mist_mode = GetMistMode();
            if(new == EVENT_MIST_KEY_SHORT){
                Do4KeyMistPressed();
            }
            else if (new == EVENT_IR_MIST){
                Do4KeyMistPressed();
            }
            else if (new == EVENT_WIFI_MIST_POWER_OFF){
                StopMisting();
            }
            else if (new == EVENT_WIFI_MIST_MODE){
                if (mist_mode == continuous){
                    SwitchMistMode(intermittent);
                }
                else if (mist_mode == intermittent){
                    SwitchMistMode(continuous);
                }
                else {}
            }
            else if (new == EVENT_WIFI_HM04_POWER_OFF){
                TurnOffHM04();
            }
            else {}
        }
        else{
            if(new == EVENT_MIST_KEY_SHORT){
                Do4KeyMistPressed();
            }
            else if (new == EVENT_IR_MIST){
                Do4KeyMistPressed();
            }
            else if (new == EVENT_WIFI_MIST_POWER_ON){
                StartMisting();
            }
            else {}
        }

        /// when speaker is bt mode
        if (GetSpeakerMode() == bt){
            if (IsLampTurnedOn()){
                if (new == EVENT_IR_SCENARIO 
                || new == EVENT_WIFI_LAMP_SCENARIO){
                    SwitchToLinein();
                } 
                else if (new == EVENT_WIFI_LAMP_MODE){
                    
                    if (GetLampMode() == scenario_mode){
                        SwitchToLinein();
                    }
                }
                else if (new == EVENT_WIFI_LAMP_POWER_ON){
                    if (GetLampMode() == scenario_mode){
                        SwitchToLinein();
                    }
                }
            }
            
                
        }
        else {  ///< when speaker is linein mode
            if (new == EVENT_IR_LAMP
            || new == EVENT_IR_DYNAMIC
            || new == EVENT_IR_WHITE
            || new == EVENT_IR_YELLOW
            || new == EVENT_WIFI_LAMP_POWER_ON
            || new == EVENT_WIFI_LAMP_POWER_OFF
            // || EVENT_WIFI_LAMP_MODE
            || new == EVENT_WIFI_LAMP_STATIC_COLOR_R
            || new == EVENT_WIFI_LAMP_STATIC_COLOR_G
            || new == EVENT_WIFI_LAMP_STATIC_COLOR_B
            || new == EVENT_WIFI_LAMP_BRIGHTNESS){
                SwitchToBT();
            }
            else if (new == EVENT_WIFI_LAMP_MODE){
                
                if (GetLampMode() != scenario_mode){
                    SwitchToBT();
                }
            }
        }

        /// whether hm04 is turned on or not.
        if (new == EVENT_PAIR_KEY_LONG){
            /// start pairing
            // gizwitsSetMode(WIFI_AIRLINK_MODE);
            gizwitsSetMode(WIFI_SOFTAP_MODE);
            /// show the pair hmi
            StartPairing_hmi();
            SetWifiStatus(wifi_pairing);
        }
        else if (new == EVENT_IR_PLAY_PAUSE){
            #ifdef PRINT_FSM
                printf(">>>> FSM: ir paly/pause.\n");
            #endif
            PlayPause();
        }
        else if (new == EVENT_IR_VOLUME_PLUS){
            #ifdef PRINT_FSM
                printf(">>>> FSM: ir volume plus.\n");
            #endif
            VolumPlus();
        }
        else if (new == EVENT_IR_VOLUME_MINUS){
            #ifdef PRINT_FSM
                printf(">>>> FSM: ir volume Minus.\n");
            #endif
            VolumMinus();
        }

        if  (new == EVENT_STOP_PAIRING){
            /// stop pair hmi 
        }
    }
}


static void FSM_LAMP_ON_REGULAR(lamp_mode_t mode, event_t event){
    uint8_t brightness_percentage;
    color_group_t color;
   if(event == EVENT_LAMP_KEY_SHORT){
       Do4KeyLampPressed();
   }

   else if (event == EVENT_IR_LAMP){
       #ifdef PRINT_FSM
        printf(">>>> FSM: ir lamp on/off.\n");
       #endif
       TurnOffLamp();
   }
   else if (event == EVENT_IR_WHITE){
       #ifdef PRINT_FSM
        printf(">>>> FSM: ir white.\n");
       #endif
       if (mode != white_mode) {
           SetLampWhite(LAMP_BRIGHTNESS_LEVEL_2);
       }
       else {
            brightness_percentage = GetLampBrightness();
            if (brightness_percentage < LAMP_BRIGHTNESS_LEVEL_1){
                SetLampWhite(LAMP_BRIGHTNESS_LEVEL_1);
            }   
            else if (brightness_percentage < LAMP_BRIGHTNESS_LEVEL_2){
                SetLampWhite(LAMP_BRIGHTNESS_LEVEL_2);
            }
            else if (brightness_percentage < LAMP_BRIGHTNESS_LEVEL_3){
                SetLampWhite(LAMP_BRIGHTNESS_LEVEL_3);
            }
            else {
                SetLampWhite(LAMP_BRIGHTNESS_LEVEL_1);
            }
       }
       
   }
   else if (event == EVENT_IR_YELLOW){
       #ifdef PRINT_FSM
        printf(">>>> FSM: ir yellow.\n");
       #endif
       if (mode != yellow_mode){
           SetLampYellow(LAMP_BRIGHTNESS_LEVEL_2);
       }
       else {
            brightness_percentage = GetLampBrightness();
            if (brightness_percentage < LAMP_BRIGHTNESS_LEVEL_1){
                SetLampYellow(LAMP_BRIGHTNESS_LEVEL_1);
            }   
            else if (brightness_percentage < LAMP_BRIGHTNESS_LEVEL_2){
                SetLampYellow(LAMP_BRIGHTNESS_LEVEL_2);
            }
            else if (brightness_percentage < LAMP_BRIGHTNESS_LEVEL_3){
                SetLampYellow(LAMP_BRIGHTNESS_LEVEL_3);
            }
            else {
                SetLampYellow(LAMP_BRIGHTNESS_LEVEL_1);
            }
       }
   }
   else if (event == EVENT_IR_DYNAMIC){
       #ifdef PRINT_FSM
        printf(">>>> FSM: ir dynamic.\n");
       #endif
       if (mode != dynamic_mode){
           SwitchLampMode(dynamic_mode);
       }
       else {
           SwitchLampMode(static_mode);
       }
       
   }
   else if (event == EVENT_IR_SCENARIO){
       #ifdef PRINT_FSM
        printf(">>>> FSM: ir scenario--");
       #endif
       
       if (mode != scenario_mode){
           SwitchLampMode(scenario_mode);
       }
       else {
           lamp_scenario_t scenario_l;
           scenario_l = GetLampScenario();
           if (scenario_l == tropical){
               scenario_l = morninig;
           }
           else {
               scenario_l++;
           }
           #ifdef PRINT_FSM
           switch(scenario_l){
               case morninig:
               printf("morning\n");
               break;
               case dream:
               printf("dream\n");
               break;
               case romantic:
               printf("romantic\n");
               break;
               case ocean:
               printf("ocean\n");
               break;
               case nature:
               printf("nature\n");
               break;
               case tropical:
               printf("tropical\n");
               break;
               default:
               break;
           }
           #endif
           SwitchLampScenario(scenario_l);
       }
       
   }
   else if (event == EVENT_WIFI_LAMP_POWER_OFF){
       #ifdef PRINT_FSM
        printf(">>>> FSM: wifi lamp power.\n");
       #endif
       TurnOffLamp();
   }
   else if (event == EVENT_WIFI_LAMP_MODE){
       #ifdef PRINT_FSM
        printf(">>>> FSM: wifi lamp mode.\n");
       #endif
        lamp_mode_t mode;
        mode = currentDataPoint.valuelamp_mode;
        SwitchLampMode(mode);
   }
   else if (event == EVENT_WIFI_LAMP_BRIGHTNESS){
       uint8_t brightness;
       brightness = currentDataPoint.valuelamp_brightness_percent;
       if (mode == yellow_mode){
           SetLampYellow(brightness);
       }
       else if (mode == white_mode){
           SetLampWhite(brightness);
       }
       else {}
   }
   else if (event == EVENT_WIFI_LAMP_SCENARIO){
       #ifdef PRINT_FSM
        printf(">>>> FSM: wifi lamp scenario--");
       #endif
       lamp_scenario_t scenario;
       scenario = currentDataPoint.valuescenario;
       #ifdef PRINT_FSM
           switch(scenario){
               case morninig:
               printf("morning\n");
               break;
               case dream:
               printf("dream\n");
               break;
               case romantic:
               printf("romantic\n");
               break;
               case ocean:
               printf("ocean\n");
               break;
               case nature:
               printf("nature\n");
               break;
               case tropical:
               printf("tropical\n");
               break;
               default:
               break;
           }
           #endif
       SwitchLampScenario(scenario);
   }
   else if (event == EVENT_WIFI_LAMP_STATIC_COLOR_R){
        
        color = GetLampColor();
        color.r = currentDataPoint.valuelamp_static_color_r;
        SetLampColor(color);
        SwitchLampMode(static_mode);
   }
   else if (event == EVENT_WIFI_LAMP_STATIC_COLOR_G){
        color = GetLampColor();
        color.g = currentDataPoint.valuelamp_static_color_g;
        SetLampColor(color);
        SwitchLampMode(static_mode);
   }
   else if (event == EVENT_WIFI_LAMP_STATIC_COLOR_B){
        color = GetLampColor();
        color.b = currentDataPoint.valuelamp_static_color_b;
        SetLampColor(color);
        SwitchLampMode(static_mode);
   }
   else if (event == EVENT_WIFI_HM04_POWER_OFF){
       TurnOffHM04();

   }
   else {}
}
