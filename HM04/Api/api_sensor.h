/**
 * @brief 
 * 
 * @file api_sensor.h
 * @author your name
 * @date 2018-09-11
 */

#ifndef API_SENSOR_H
#define API_SENSOR_H

#include <stdint.h>

typedef enum {
    celsius,
    fahrenheit
}temperature_unit_t;

temperature_unit_t GetTemperatureUnit(void);
uint8_t SetTemperatureUnit(temperature_unit_t unit);
uint8_t GetHumidityAndTemperature(uint32_t * humidity, uint32_t * temperature);



#endif