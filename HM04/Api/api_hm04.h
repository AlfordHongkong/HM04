/**
 * @brief 
 * 
 * @file api_hm04.h
 * @author your name
 * @date 2018-09-11
 */


#ifndef API_HM04_H
#define API_HM04_H

#include "api_speaker.h"
#include "api_lamp.h"
#include "api_mist.h"

/**
 * @brief hm04 on or off
 * 
 */
typedef enum {
    hm04_off,
    hm04_on,
    
}hm04_status_t;

/**
 * @brief struct for water tank
 * 
 */
typedef struct {
    uint8_t flag_water_shortage : 1;
    uint8_t remaining_water_percent : 7;
}water_tank_t;

typedef enum{
    wifi_pairing,
    wifi_connected,
    wifi_disconnected,
}wifi_status_t;

typedef enum{
    hdc_connected,
    hdc_disconnected,
}hdc_status_t;

typedef struct{
    float temperature;
    float humidity;
    hdc_status_t hdc_status;
}hdc1080_t;

/**
 * @brief type for hm04
 * 
 */
typedef struct {
    hm04_status_t status;
    lamp_t * lamp;
    mist_t * mist;
    speaker_t * speaker;
    water_tank_t * water_tank;
    wifi_status_t wifi_status;
    hdc1080_t hdc1080;
}hm04_t;

/**
 * @brief Initialize the whole device
 * 
 * @return uint8_t 
 */
uint8_t InitHM04(void);

uint8_t SetHm04Status(hm04_status_t status);
hm04_status_t GetHm04Status(void);

uint8_t TurnOnHM04(void);
uint8_t TurnOffHM04(void);
uint8_t IsHm04TurnedOn(void);
uint8_t SetWifiStatus(wifi_status_t status);
wifi_status_t GetWifiStatus(void);

/// below are for hdc1080
uint8_t GetHDC1080(float *temperature, float *humidify);
uint8_t SetHDC1080(float *temperature, float *humidify);
float GetHumidity(void);
float GetTemperature(void);
hdc_status_t GetHDC1080Status(void);
uint8_t SetHDC1080Status(hdc_status_t status);


#endif
