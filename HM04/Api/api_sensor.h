/**
 * @brief 
 * 
 * @file api_sensor.h
 * @author Rock Deng
 * 
 * @note - temp ramge: 15c - 26c
 *                     60f - 80f
 *         humi range: 20%RH - 70%RH
 * @date 2018-09-11
 */

#ifndef API_SENSOR_H
#define API_SENSOR_H



#include <stdint.h>


/**
 * @brief hdc status
 * 
 */
typedef enum{
    hdc_connected,
    hdc_disconnected,
}hdc_status_t;

/**
 * @brief temperature unit
 * 
 */
typedef enum {
    celsius,
    fahrenheit
}temperature_unit_t;

/**
 * @brief hdc1080 structure
 * 
 */
typedef struct{
    float temperature;  ///< for recording temperature
    float humidity;     ///< for recording humidity
    temperature_unit_t temp_unit;   ///< for marking temp unit
    hdc_status_t hdc_status;    ///< hdc1080 module status
}hdc1080_t;

// temperature_unit_t GetTemperatureUnit(void);
// uint8_t SetTemperatureUnit(temperature_unit_t unit);
// uint8_t GetHumidityAndTemperature(uint32_t * humidity, uint32_t * temperature);

/**
 * @brief Initialization HDC module
 * 
 */
void InitHDC1080(void);

/**
 * @brief Get pointer of hdc1080 data
 * 
 * @return hdc1080_t* 
 */
hdc1080_t *GetHDC1080(void);


/**
 * @brief Get the Temperature & Humidity from hdc1080 module
 * 
 * This function should be called periodically
 * 
 * @param temp_f - temperature data
 * @param humi_f - humidity data
 * @return unsigned char - 0 means success
 */
unsigned char GetTemperatureHumidity(float *temp_f, float *humi_f);


/**
 * @brief record/update the temperature and humidity 
 * 
 * @param temperature 
 * @param humidify 
 * @return uint8_t 
 */
uint8_t UpdateTempHumi(float *temperature, float *humidify);


/**
 * @brief Get the status of hdc module, connected or disconnected
 * 
 * @return hdc_status_t 
 */
hdc_status_t GetHDC1080Status(void);

/**
 * @brief 
 * @param status 
 * @return uint8_t 
 */
uint8_t SetHDC1080Status(hdc_status_t status);

/**
 * @brief Get the Humidity 
 * 
 * @return float 
 */
float GetHumidity(void);

/**
 * @brief Get the Temperature 
 * 
 * @return float 
 */
float GetTemperature(void);


/**
 * @brief detect if temp is below the minimum threshold
 * 
 * @return uint8_t 
 */
uint8_t IsTempTooLow();

/**
 * @brief detect if temperature is upper the maximum threshold
 * 
 * @return uint8_t 
 */
uint8_t IsTempTooHigh();

/**
 * @brief detect if humidity is below the minimum threshold
 * 
 * @return uint8_t 
 */
uint8_t IsTempTooLow();

/**
 * @brief detect if humidity is upper the maximum threshold
 * 
 * @return uint8_t 
 */
uint8_t IsTempTooHigh();


#endif

