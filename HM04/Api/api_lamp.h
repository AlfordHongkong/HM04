/**
 * @brief 
 * 
 * @file api_lamp.h
 * @author Rock Deng
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

/**
 * @brief Get the Lamp Status object
 * 
 * @return lamp_status_t 
 */
lamp_status_t GetLampStatus(void);

/**
 * @brief detect if lamp is turned off or on
 * 
 * @return uint8_t - 1 means on
 */
uint8_t IsLampTurnedOn(void);

/**
 * @brief Set the Lamp Brightness 
 *        only when lamp is white mode or yellow mode,
 *         the brightness can be modified.
 * 
 * @return uint8_t - 0 means return success
 */
uint8_t SetLampBrightness(uint8_t);

/**
 * @brief turn on lamp and resume the last setting
 * 
 * @return uint8_t - 0 means return success
 */
uint8_t TurnOnLamp(void);

/**
 * @brief turn off lamp
 * 
 * @return uint8_t - 0 means success
 */
uint8_t TurnOffLamp(void);

/**
 * @brief modify the mode of lamp
 * 
 * @param mode - mode to be set
 * @return uint8_t - 0 means return success
 */
uint8_t SwitchLampMode(lamp_mode_t mode);

/**
 * @brief lamp dynamic func 
 * @note it should be called in a rtos soft timer
 * 
 */
void LampDynamicCallbackFromApiLamp(void);

/**
 * @brief start the soft-timer of lamp dynamic mode
 * 
 * @return uint8_t - 0 means return success
 */
uint8_t StartDynamicMode(void);

/**
 * @brief stop the soft-timer of lamp dynamic mode
 * 
 * @return uint8_t - 0 means return success
 */
uint8_t StopDynamicMode(void);

/**
 * @brief function for reacting the control board
 * @note each time user press the button on the control board,
 *      system will call this function.
 * 
 */
void Do4KeyLampPressed(void);

/**
 * @brief Get the Lamp Color object
 * 
 * @return color_group_t 
 */
color_group_t GetLampColor(void);

/**
 * @brief Set the Lamp Color object
 * 
 * @param color 
 * @return uint8_t 
 */
uint8_t SetLampColor(color_group_t color);

/**
 * @brief Set the Lamp White object
 * 
 * @param brightness 
 * @return uint8_t 
 */
uint8_t SetLampWhite(uint8_t brightness);

/**
 * @brief Set the Lamp Yellow object
 * 
 * @param brightness 
 * @return uint8_t 
 */
uint8_t SetLampYellow(uint8_t brightness);

/**
 * @brief Set the Scenario object
 * 
 * @param scenario 
 * @return uint8_t 
 */
uint8_t SetScenario(scenario_t scenario);



#endif
