

#include "api_sensor.h"




float CalculateTemperature(uint16_t temperature_from_hdc1080){
	float temperature;
	temperature = (float)temperature_from_hdc1080 * 165 /65536 - 40;
	return temperature;
}

float CalculateHumidity(uint16_t humidity_from_hdc1080){
	float humidity;
	humidity = (float)humidity_from_hdc1080 /65536;
	return humidity;
}

//#define PRINT_TEMPERATURE_N_HUMIDITY
unsigned char GetTemperatureHumidity(float *temp_f, float *humi_f){
	uint16_t temp, humi;
		
	if (ReadHDC1080_sw_iic(&temp, &humi)){
			// means error show up
		return 1;
	}
	
	*temp_f = CalculateTemperature(temp);
	*humi_f = CalculateHumidity(humi);
	return 0;

}
