/**
 * @file gizwits.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include "gizwits.h"
#include "gizwits_product.h"
#include "stdio.h"


void InitGizwits(void){

    timerInit();
	uartInit();

	userInit();
	gizwitsInit();
	//keyInit();
	GIZWITS_LOG("Gizwits initialization Success \n");
}























































/*=========================================================================
					gizwitsEventProcess backup		
=========================================================================*/

// /**@} */
// /**@name Gizwits User Interface
// * @{
// */

// /**
// * @brief Event handling interface

// * Description:

// * 1. Users can customize the changes in WiFi module status

// * 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

// * @param [in] info: event queue
// * @param [in] data: protocol data
// * @param [in] len: protocol data length
// * @return NULL
// * @ref gizwits_protocol.h
// */
// int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
// {
//   uint8_t i = 0;
//   dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
//   moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
//   protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
// #if MODULE_TYPE
//   gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
// #else
//   moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
// #endif

//   if((NULL == info) || (NULL == gizdata))
//   {
//     return -1;
//   }

//   for(i=0; i<info->num; i++)
//   {
//     switch(info->event[i])
//     {
//       case EVENT_lamp_status:
//         currentDataPoint.valuelamp_status = dataPointPtr->valuelamp_status;
//         GIZWITS_LOG("Evt: EVENT_lamp_status %d \n", currentDataPoint.valuelamp_status);
//         if(0x01 == currentDataPoint.valuelamp_status)
//         {
//           //user handle
//           TurnOnLamp();
//         }
//         else
//         {
//           //user handle   
//           TurnOffLamp(); 
//         }
//         break;
//       case EVENT_mist_status:
//         currentDataPoint.valuemist_status = dataPointPtr->valuemist_status;
//         GIZWITS_LOG("Evt: EVENT_mist_status %d \n", currentDataPoint.valuemist_status);
//         if(0x01 == currentDataPoint.valuemist_status)
//         {
//           //user handle
//           StartMisting();
//         }
//         else
//         {
//           //user handle    
//           StopMisting();
//         }
//         break;
//       case EVENT_hm04_status:
//         currentDataPoint.valuehm04_status = dataPointPtr->valuehm04_status;
//         GIZWITS_LOG("Evt: EVENT_hm04_status %d \n", currentDataPoint.valuehm04_status);
//         if(0x01 == currentDataPoint.valuehm04_status)
//         {
//           //user handle
//         }
//         else
//         {
//           //user handle   
//           /// it's general off 
//           TurnOffLamp();
//           StopMisting();
//         }
//         break;

//       case EVENT_temperature_unit:
//         currentDataPoint.valuetemperature_unit = dataPointPtr->valuetemperature_unit;
//         GIZWITS_LOG("Evt: EVENT_temperature_unit %d\n", currentDataPoint.valuetemperature_unit);
//         switch(currentDataPoint.valuetemperature_unit)
//         {
//           case temperature_unit_VALUE0:
//             //user handle
//             break;
//           case temperature_unit_VALUE1:
//             //user handle
//             break;
//           default:
//             break;
//         }
//         break;
//       case EVENT_lamp_mode:
//         currentDataPoint.valuelamp_mode = dataPointPtr->valuelamp_mode;
//         GIZWITS_LOG("Evt: EVENT_lamp_mode %d\n", currentDataPoint.valuelamp_mode);
//         lamp_mode_t mode;
//         mode = currentDataPoint.valuelamp_mode;
//         SwitchLampMode(mode);
//         // switch(currentDataPoint.valuelamp_mode)
//         // {
//         //   case lamp_mode_VALUE0:
//         //     //user handle
//         //     break;
//         //   case lamp_mode_VALUE1:
//         //     //user handle
//         //     break;
//         //   case lamp_mode_VALUE2:
//         //     //user handle
//         //     break;
//         //   default:
//         //     break;
//         // }
//         break;
//       case EVENT_scenario:
//         currentDataPoint.valuescenario = dataPointPtr->valuescenario;
//         GIZWITS_LOG("Evt: EVENT_scenario %d\n", currentDataPoint.valuescenario);
//         switch(currentDataPoint.valuescenario)
//         {
//           case scenario_VALUE0:
//             //user handle
//             break;
//           case scenario_VALUE1:
//             //user handle
//             break;
//           case scenario_VALUE2:
//             //user handle
//             break;
//           case scenario_VALUE3:
//             //user handle
//             break;
//           case scenario_VALUE4:
//             //user handle
//             break;
//           case scenario_VALUE5:
//             //user handle
//             break;
//           default:
//             break;
//         }
//         break;
//       case EVENT_mist_mode:
//         currentDataPoint.valuemist_mode = dataPointPtr->valuemist_mode;
//         GIZWITS_LOG("Evt: EVENT_mist_mode %d\n", currentDataPoint.valuemist_mode);
//         mist_mode_t mistMode;
//         mistMode = currentDataPoint.valuemist_mode;
//         SwitchMistMode(mistMode);
//         // switch(currentDataPoint.valuemist_mode)
//         // {
//         //   case mist_mode_VALUE0:
//         //     //user handle
//         //     break;
//         //   case mist_mode_VALUE1:
//         //     //user handle
//         //     break;
//         //   default:
//         //     break;
//         // }
//         break;
//       case EVENT_mist_timer:
//         currentDataPoint.valuemist_timer = dataPointPtr->valuemist_timer;
//         GIZWITS_LOG("Evt: EVENT_mist_timer %d\n", currentDataPoint.valuemist_timer);
//         mist_timer_t timer;
//         timer = currentDataPoint.valuemist_timer;
//         SwitchMistTimer(timer);
//         // switch(currentDataPoint.valuemist_timer)
//         // {
//         //   case mist_timer_VALUE0:
//         //     //user handle
//         //     break;
//         //   case mist_timer_VALUE1:
//         //     //user handle
//         //     break;
//         //   case mist_timer_VALUE2:
//         //     //user handle
//         //     break;
//         //   default:
//         //     break;
//         // }
//         break;
//       case EVENT_speaker_status:
//         currentDataPoint.valuespeaker_status = dataPointPtr->valuespeaker_status;
//         GIZWITS_LOG("Evt: EVENT_speaker_status %d\n", currentDataPoint.valuespeaker_status);
//         switch(currentDataPoint.valuespeaker_status)
//         {
//           case speaker_status_VALUE0:
//             //user handle
//             break;
//           case speaker_status_VALUE1:
//             //user handle
//             break;
//           default:
//             break;
//         }
//         break;

//       case EVENT_lamp_static_color_r:
//         currentDataPoint.valuelamp_static_color_r = dataPointPtr->valuelamp_static_color_r;
//         GIZWITS_LOG("Evt:EVENT_lamp_static_color_r %d\n",currentDataPoint.valuelamp_static_color_r);
//         //user handle
//         color_group_t color;
//         color = GetLampColor();
//         color.r = currentDataPoint.valuelamp_static_color_r;
//         SetLampColor(color);
//         SwitchLampMode(static_mode);
//         break;
//       case EVENT_lamp_static_color_g:
//         currentDataPoint.valuelamp_static_color_g = dataPointPtr->valuelamp_static_color_g;
//         GIZWITS_LOG("Evt:EVENT_lamp_static_color_g %d\n",currentDataPoint.valuelamp_static_color_g);
//         //user handle
//         color = GetLampColor();
//         color.g = currentDataPoint.valuelamp_static_color_g;
//         SetLampColor(color);
//         SwitchLampMode(static_mode);
//         break;
//       case EVENT_lamp_static_color_b:
//         currentDataPoint.valuelamp_static_color_b = dataPointPtr->valuelamp_static_color_b;
//         GIZWITS_LOG("Evt:EVENT_lamp_static_color_b %d\n",currentDataPoint.valuelamp_static_color_b);
//         //user handle
//         color = GetLampColor();
//         color.b = currentDataPoint.valuelamp_static_color_b;
//         SetLampColor(color);
//         SwitchLampMode(static_mode);
//         break;
//       case EVENT_lamp_brightness_percent:
//         currentDataPoint.valuelamp_brightness_percent = dataPointPtr->valuelamp_brightness_percent;
//         GIZWITS_LOG("Evt:EVENT_lamp_brightness_percent %d\n",currentDataPoint.valuelamp_brightness_percent);
//         //user handle
//         break;
//       case EVENT_speaker_volum_percent:
//         currentDataPoint.valuespeaker_volum_percent = dataPointPtr->valuespeaker_volum_percent;
//         GIZWITS_LOG("Evt:EVENT_speaker_volum_percent %d\n",currentDataPoint.valuespeaker_volum_percent);
//         //user handle
//         break;


//       case WIFI_SOFTAP:
//         GIZWITS_LOG("SoftAP event.\n");
//         break;
//       case WIFI_AIRLINK:
//         GIZWITS_LOG("AirLink event.\n");
//         break;
//       case WIFI_STATION:
//         break;
//       case WIFI_CON_ROUTER:
 
//         break;
//       case WIFI_DISCON_ROUTER:
 
//         break;
//       case WIFI_CON_M2M:
 
//         break;
//       case WIFI_DISCON_M2M:
//         break;
//       case WIFI_RSSI:
//         GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
//         break;
//       case TRANSPARENT_DATA:
//         GIZWITS_LOG("TRANSPARENT_DATA \n");
//         //user handle , Fetch data from [data] , size is [len]
//         break;
//       case WIFI_NTP:
//         GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
//         break;
//       case MODULE_INFO:
//             GIZWITS_LOG("MODULE INFO ...\n");
//       #if MODULE_TYPE
//             GIZWITS_LOG("GPRS MODULE ...\n");
//             //Format By gprsInfo_t
//       #else
//             GIZWITS_LOG("WIF MODULE ...\n");
//             //Format By moduleInfo_t
//             GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
//       #endif
//     break;
//       default:
//         break;
//     }
//   }

//   return 0;
// }