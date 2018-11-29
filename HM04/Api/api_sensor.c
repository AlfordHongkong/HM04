

#include "api_sensor.h"
#include "sw_iic.h"

hdc1080_t hdc1080;

void InitHDC1080(void){
    hdc1080.humidity = 0;
    hdc1080.temperature = 0;
    hdc1080.temp_unit = celsius;
    hdc1080.hdc_status = hdc_disconnected;
}

hdc1080_t *GetHDC1080(void){
    return &hdc1080;
}

/**
 * @brief convert the original data from hdc module to human readable data
 * 
 * @param temperature_from_hdc1080 
 * @return float 
 */
float CalculateTemperature(uint16_t temperature_from_hdc1080){
	float temperature;
	temperature = (float)temperature_from_hdc1080 * 165 /65536 - 40;
	return temperature;
}


/**
 * @brief convert the original data from hdc module to human readable data
 * 
 * @param humidity_from_hdc1080 
 * @return float 
 */
float CalculateHumidity(uint16_t humidity_from_hdc1080){
	float humidity;
	humidity = (float)humidity_from_hdc1080 /65536;
	return humidity;
}

//#define PRINT_TEMPERATURE_N_HUMIDITY
unsigned char GetTemperatureHumidity(float *temp_f, float *humi_f){
	static uint8_t error_count = 0; 
    uint16_t temp, humi;
		
	if (ReadHDC1080_sw_iic(&temp, &humi)){
		/// means error show up
        if (error_count >= 10){
            hdc1080.hdc_status = hdc_disconnected;
        }
        else {
            error_count++;
        }
		return 1;
	}
    else {
        /// read hdc module successd
        hdc1080.hdc_status = hdc_connected;
        if (error_count > 0){
            error_count--;
        }
    }
	
	*temp_f = CalculateTemperature(temp);
	*humi_f = CalculateHumidity(humi);
	return 0;

}


hdc_status_t GetHDC1080Status(void){
    return hdc1080.hdc_status;
}

uint8_t SetHDC1080Status(hdc_status_t status){
    hdc1080.hdc_status = status;

    return 1;
}

float GetHumidity(void){
    return hdc1080.humidity;
}
float GetTemperature(void){
    return hdc1080.temperature;
}


#define TEMP_LOWEST 15
#define TEMP_HIGHEST 25
#define HUMI_LOWSET 20
#define HUMI_HIGHEST 70

uint8_t IsTempTooLow(void){
    if (hdc1080.temperature < TEMP_LOWEST){
        return 1;
    }

    return 0;
}

uint8_t IsTempTooHigh(void){
    if (hdc1080.temperature > TEMP_HIGHEST) {
        return 1;

    }

    return 0;
}

uint8_t IsHumiTooLow(void){
    if (hdc1080.humidity < HUMI_LOWSET) {
        return 1;
    }

    return 0;
}

uint8_t IsHumiTooHigh(void){
    if (hdc1080.humidity > HUMI_HIGHEST) {
        return 1;
    }

    return 0;
}


uint8_t UpdateTempHumi(float *temperature, float *humidity){
    hdc1080.temperature = *temperature;
    hdc1080.humidity = *humidity;

    return 1;
}
