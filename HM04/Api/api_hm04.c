/**
 * @file api_hm04.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */


#include "api_hm04.h"
#include "bsp_hm04.h"

static hm04_t hm04;

uint8_t InitHM04(void){
    /// first, initialize the bsp
    InitBSP();
    /// second, initialize api of each component
    InitLamp();
    InitMist();

    /// initialize hm04
    hm04.lamp = GetLamp();
    hm04.mist = GetMist();
    hm04.status = hm04_off;
    hm04.wifi_status = wifi_disconnected;
    hm04.hdc1080.temperature = 0;
    hm04.hdc1080.humidity = 0;
    hm04.hdc1080.hdc_status = hdc_disconnected;

    return 1;
}

uint8_t SetHm04Status(hm04_status_t status){
    hm04.status = status;

    return 1;
}

hm04_status_t GetHm04Status(void){
    return hm04.status;
}


uint8_t TurnOnHM04(void){

    /// 
    hm04.status = hm04_on;
    return 1;
}


uint8_t TurnOffHM04(void){
    /// turn off lamp
    TurnOffLamp();
    /// turn off mist   
    StopMisting();

    /// modify flag data
    hm04.status = hm04_off;

    return 1;
}

uint8_t IsHm04TurnedOn(void){
    if (hm04.status == hm04_on)
        return 1;
    
    return 0;
}

uint8_t SetWifiStatus(wifi_status_t status){
    hm04.wifi_status = status;

    return 1;
}

wifi_status_t GetWifiStatus(void){
    return hm04.wifi_status;
    
}

uint8_t GetHDC1080(float *temperature, float *humidify){
    *temperature = hm04.hdc1080.temperature;
    *humidify = hm04.hdc1080.humidity;

    return 1;
}

uint8_t SetHDC1080(float *temperature, float *humidity){
    hm04.hdc1080.temperature = *temperature;
    hm04.hdc1080.humidity = *humidity;

    return 1;
}


float GetHumidity(void){
    return hm04.hdc1080.humidity;
}
float GetTemperature(void){
    return hm04.hdc1080.temperature;
}
hdc_status_t GetHDC1080Status(void){
    return hm04.hdc1080.hdc_status;
}

uint8_t SetHDC1080Status(hdc_status_t status){
    hm04.hdc1080.hdc_status = status;

    return 1;
}
