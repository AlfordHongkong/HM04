/**
 * @brief 
 * 
 * @file api_lamp.h
 * @author your name
 * @date 2018-09-11
 */

#ifndef API_LAMP_H
#define API_LAMP_H

#include <stdint.h>
#include "bsp_lamp.h"
/*==============================================================
                        Data types
==============================================================*/
/**
 * @brief lamp status
 * 
 */
typedef enum {
    lamp_off,
    lamp_on,
    
    lamp_status_max
}lamp_status_t;

/**
 * @brief lamp mode
 * 
 */
typedef enum {
    static_mode,
    dynamic_mode,
    scenario_mode,
    white_mode,
    yellow_mode,
    max_mode
}lamp_mode_t;

/**
 * @brief rbg color data
 * 
 */
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
}color_group_t;

typedef enum {
    morninig,
    dream,
    romantic,
    ocean,
    nature,
    tropical,
    scenario_max
}scenario_t;
/**
 * @brief lamp stucture
 * 
 */
typedef struct {
    lamp_status_t status;
    lamp_mode_t mode;
    color_group_t static_color;
    scenario_t scenario;
    uint8_t brightness_percent;
    
} lamp_t;


/*==============================================================
                        Function declare
==============================================================*/

/**
 * @brief Initialize the data related with lamp
 * 
 */
void InitLamp(void);

/**
 * @brief Get the Lamp object
 * 
 * @return lamp_t* 
 */
lamp_t* GetLamp(void);
lamp_status_t GetLampStatus(void);
uint8_t IsLampTurnedOn(void);

uint8_t SetLampBrightness(uint8_t);
uint8_t TurnOnLamp(void);
uint8_t TurnOffLamp(void);
uint8_t SwitchLampMode(lamp_mode_t mode);
void LampDynamicCallbackFromApiLamp(void);
uint8_t StartDynamicMode(void);
uint8_t StopDynamicMode(void);
void Do4KeyLampPressed(void);

color_group_t GetLampColor(void);
uint8_t SetLampColor(color_group_t color);
uint8_t SetLampWhite(uint8_t brightness);
uint8_t SetLampYellow(uint8_t brightness);
uint8_t SetScenario(scenario_t scenario);



#endif
