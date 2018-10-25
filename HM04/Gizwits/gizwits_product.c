/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <stdio.h>
#include <string.h>
// #include "hal_key.h"
#include "gizwits_product.h"
#include "common.h"

#include "gizwits.h"
#include "api_lamp.h"
#include "api_mist.h"
#include "bsp_gpio.h"

static uint32_t timerMsCount;
uint8_t aRxBuffer;

/** User area the current device state structure*/
dataPoint_t currentDataPoint;
// extern keysTypedef_t keys;

extern TIM_HandleTypeDef htim_gizwits;
extern UART_HandleTypeDef huart_debug;
extern UART_HandleTypeDef huart_wifi;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_lamp_status:
        currentDataPoint.valuelamp_status = dataPointPtr->valuelamp_status;
        GIZWITS_LOG("Evt: EVENT_lamp_status %d \n", currentDataPoint.valuelamp_status);
        if(0x01 == currentDataPoint.valuelamp_status)
        {
          //user handle
          TurnOnLamp();
        }
        else
        {
          //user handle    
          TurnOffLamp();
        }
        break;
      case EVENT_mist_status:
        currentDataPoint.valuemist_status = dataPointPtr->valuemist_status;
        GIZWITS_LOG("Evt: EVENT_mist_status %d \n", currentDataPoint.valuemist_status);
        if(0x01 == currentDataPoint.valuemist_status)
        {
          //user handle
          StartMisting();
        }
        else
        {
          //user handle    
          StopMisting();
        }
        break;
      case EVENT_hm04_status:
        currentDataPoint.valuehm04_status = dataPointPtr->valuehm04_status;
        GIZWITS_LOG("Evt: EVENT_hm04_status %d \n", currentDataPoint.valuehm04_status);
        if(0x01 == currentDataPoint.valuehm04_status)
        {
          //user handle
        }
        else
        {
          //user handle   
          /// it's general off 
          TurnOffLamp();
          StopMisting(); 
        }
        break;

      case EVENT_temperature_unit:
        currentDataPoint.valuetemperature_unit = dataPointPtr->valuetemperature_unit;
        GIZWITS_LOG("Evt: EVENT_temperature_unit %d\n", currentDataPoint.valuetemperature_unit);
        switch(currentDataPoint.valuetemperature_unit)
        {
          case temperature_unit_VALUE0:
            //user handle
            break;
          case temperature_unit_VALUE1:
            //user handle
            break;
          default:
            break;
        }
        break;
      case EVENT_lamp_mode:
        currentDataPoint.valuelamp_mode = dataPointPtr->valuelamp_mode;
        GIZWITS_LOG("Evt: EVENT_lamp_mode %d\n", currentDataPoint.valuelamp_mode);
        lamp_mode_t mode;
        mode = currentDataPoint.valuelamp_mode;
        SwitchLampMode(mode);
        // switch(currentDataPoint.valuelamp_mode)
        // {
        //   case lamp_mode_VALUE0:
        //     //user handle
        //     break;
        //   case lamp_mode_VALUE1:
        //     //user handle
        //     break;
        //   case lamp_mode_VALUE2:
        //     //user handle
        //     break;
        //   case lamp_mode_VALUE3:
        //     //user handle
        //     break;
        //   case lamp_mode_VALUE4:
        //     //user handle
        //     break;
        //   default:
        //     break;
        // }
        break;
      case EVENT_scenario:
        currentDataPoint.valuescenario = dataPointPtr->valuescenario;
        GIZWITS_LOG("Evt: EVENT_scenario %d\n", currentDataPoint.valuescenario);
        switch(currentDataPoint.valuescenario)
        {
          case scenario_VALUE0:
            //user handle
            break;
          case scenario_VALUE1:
            //user handle
            break;
          case scenario_VALUE2:
            //user handle
            break;
          case scenario_VALUE3:
            //user handle
            break;
          case scenario_VALUE4:
            //user handle
            break;
          case scenario_VALUE5:
            //user handle
            break;
          default:
            break;
        }
        break;
      case EVENT_mist_mode:
        currentDataPoint.valuemist_mode = dataPointPtr->valuemist_mode;
        GIZWITS_LOG("Evt: EVENT_mist_mode %d\n", currentDataPoint.valuemist_mode);
        mist_mode_t mistMode;
        mistMode = currentDataPoint.valuemist_mode;
        SwitchMistMode(mistMode);
        // switch(currentDataPoint.valuemist_mode)
        // {
        //   case mist_mode_VALUE0:
        //     //user handle
        //     break;
        //   case mist_mode_VALUE1:
        //     //user handle
        //     break;
        //   default:
        //     break;
        // }
        break;
      case EVENT_mist_timer:
        currentDataPoint.valuemist_timer = dataPointPtr->valuemist_timer;
        GIZWITS_LOG("Evt: EVENT_mist_timer %d\n", currentDataPoint.valuemist_timer);
        mist_timer_t timer;
        timer = currentDataPoint.valuemist_timer;
        SwitchMistTimer(timer);
        // switch(currentDataPoint.valuemist_timer)
        // {
        //   case mist_timer_VALUE0:
        //     //user handle
        //     break;
        //   case mist_timer_VALUE1:
        //     //user handle
        //     break;
        //   case mist_timer_VALUE2:
        //     //user handle
        //     break;
        //   default:
        //     break;
        // }
        break;
      case EVENT_speaker_status:
        currentDataPoint.valuespeaker_status = dataPointPtr->valuespeaker_status;
        GIZWITS_LOG("Evt: EVENT_speaker_status %d\n", currentDataPoint.valuespeaker_status);
        switch(currentDataPoint.valuespeaker_status)
        {
          case speaker_status_VALUE0:
            //user handle
            break;
          case speaker_status_VALUE1:
            //user handle
            break;
          default:
            break;
        }
        break;

      case EVENT_lamp_static_color_r:
        currentDataPoint.valuelamp_static_color_r = dataPointPtr->valuelamp_static_color_r;
        GIZWITS_LOG("Evt:EVENT_lamp_static_color_r %d\n",currentDataPoint.valuelamp_static_color_r);
        //user handle
        color_group_t color;
        color = GetLampColor();
        color.r = currentDataPoint.valuelamp_static_color_r;
        SetLampColor(color);
        SwitchLampMode(static_mode);
        break;
      case EVENT_lamp_static_color_g:
        currentDataPoint.valuelamp_static_color_g = dataPointPtr->valuelamp_static_color_g;
        GIZWITS_LOG("Evt:EVENT_lamp_static_color_g %d\n",currentDataPoint.valuelamp_static_color_g);
        //user handle
        color = GetLampColor();
        color.g = currentDataPoint.valuelamp_static_color_g;
        SetLampColor(color);
        SwitchLampMode(static_mode);
        break;
      case EVENT_lamp_static_color_b:
        currentDataPoint.valuelamp_static_color_b = dataPointPtr->valuelamp_static_color_b;
        GIZWITS_LOG("Evt:EVENT_lamp_static_color_b %d\n",currentDataPoint.valuelamp_static_color_b);
        //user handle
        color = GetLampColor();
        color.b = currentDataPoint.valuelamp_static_color_b;
        SetLampColor(color);
        SwitchLampMode(static_mode);
        break;
      case EVENT_lamp_brightness_percent:
        currentDataPoint.valuelamp_brightness_percent = dataPointPtr->valuelamp_brightness_percent;
        GIZWITS_LOG("Evt:EVENT_lamp_brightness_percent %d\n",currentDataPoint.valuelamp_brightness_percent);
        //user handle
        SetLampBrightness(currentDataPoint.valuelamp_brightness_percent);
        break;
      case EVENT_speaker_volum_percent:
        currentDataPoint.valuespeaker_volum_percent = dataPointPtr->valuespeaker_volum_percent;
        GIZWITS_LOG("Evt:EVENT_speaker_volum_percent %d\n",currentDataPoint.valuespeaker_volum_percent);
        //user handle
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
 
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 /*
    currentDataPoint.valuehumidity = ;//Add Sensor Data Collection
    currentDataPoint.valueremaining_water_percent = ;//Add Sensor Data Collection
    currentDataPoint.valuetemperature = ;//Add Sensor Data Collection
    currentDataPoint.valueflag_water_shortage = ;//Add Sensor Data Collection

    */
    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valuelamp_status = ;
      currentDataPoint.valuemist_status = ;
      currentDataPoint.valuehm04_status = ;
      currentDataPoint.valuetemperature_unit = ;
      currentDataPoint.valuelamp_mode = ;
      currentDataPoint.valuescenario = ;
      currentDataPoint.valuemist_mode = ;
      currentDataPoint.valuemist_timer = ;
      currentDataPoint.valuespeaker_status = ;
      currentDataPoint.valuelamp_static_color_r = ;
      currentDataPoint.valuelamp_static_color_g = ;
      currentDataPoint.valuelamp_static_color_b = ;
      currentDataPoint.valuelamp_brightness_percent = ;
      currentDataPoint.valuespeaker_volum_percent = ;
      currentDataPoint.valuehumidity = ;
      currentDataPoint.valueremaining_water_percent = ;
      currentDataPoint.valuetemperature = ;
      currentDataPoint.valueflag_water_shortage = ;
    */

}


/**
* @brief Millisecond timing maintenance function, milliseconds increment, overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief Read millisecond count

* @param none
* @return millisecond count
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief MCU reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}

/**@} */

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart_debug, (uint8_t *)&ch, 1, 0xFFFF);
 
  return ch;
}

/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim_gizwits)
	{
			//keyHandle((keysTypedef_t *)&keys);
			gizTimerMs();
	}
}

/**
* @brief Timer TIM3 init function

* @param none
* @return none
*/
void timerInit(void)
{
	HAL_TIM_Base_Start_IT(&htim_gizwits);
}

/**
  * @brief  This function handles USART IDLE interrupt.
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef*UartHandle)  
{  
    if(UartHandle->Instance == USART_WIFI)  
    {  
				gizPutData((uint8_t *)&aRxBuffer, 1);

        HAL_UART_Receive_IT(&huart_wifi, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断  
    }  
}  

/**
* @brief USART init function

* Serial communication between WiFi modules and device MCU
* @param none
* @return none
*/
void uartInit(void)
{
	HAL_UART_Receive_IT(&huart_wifi, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断  
}

/**
* @brief Serial port write operation, send data to WiFi module
*
* @param buf      : buf address
* @param len      : buf length
*
* @return : Return effective data length;-1，return failure
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
		uint8_t crc[1] = {0x55};
    uint32_t i = 0;
	
    if(NULL == buf)
    {
        return -1;
    }

    for(i=0; i<len; i++)
    {
        HAL_UART_Transmit_IT(&huart_wifi, (uint8_t *)&buf[i], 1);
				while (huart_wifi.gState != HAL_UART_STATE_READY);//Loop until the end of transmission

        if(i >=2 && buf[i] == 0xFF)
        {
						HAL_UART_Transmit_IT(&huart_wifi, (uint8_t *)&crc, 1);
						while (huart_wifi.gState != HAL_UART_STATE_READY);//Loop until the end of transmission
        }
    }

#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if(i >=2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }
    GIZWITS_LOG("\n");
#endif
		
		return len;
}  
