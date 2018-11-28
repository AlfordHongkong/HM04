/**
 * @brief 
 * 
 * @file api_sensor.h
 * @author your name
 * 
 * @note - temp ramge: 15c - 26c
 *                     60f - 80f
 *         humi range: 20%RH - 70%RH
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

/**
 * @brief Get the Temperature Humidity object
 * 
 * This function should be called periodically
 * 
 * @param temp_f - temperature data
 * @param humi_f - humidity data
 * @return unsigned char - 0 means success
 */
unsigned char GetTemperatureHumidity(float *temp_f, float *humi_f);


#endif