/**
 * @file fsm.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2018-10-22
 * 
 * @copyright Copyright (c) 2018
 * 
 */




#ifndef FSM_H
#define FSM_H

typedef enum {
    /// control panel events
    EVENT_PAIR_KEY_SHORT = 1,
    EVENT_PAIR_KEY_LONG,
    EVENT_LAMP_KEY_SHORT,
    EVENT_LAMP_KEY_LONG,
    EVENT_MIST_KEY_SHORT,
    EVENT_MIST_KEY_LONG,
	EVENT_STOP_PAIRING,
    /// ir remote events
    EVENT_IR_LAMP,
    EVENT_IR_MIST,
    EVENT_IR_WHITE,
    EVENT_IR_YELLOW,
    EVENT_IR_DYNAMIC,
    EVENT_IR_SCENARIO,
    EVENT_IR_VOLUME_PLUS,
    EVENT_IR_VOLUME_MINUS,
    EVENT_IR_PLAY_PAUSE,
    /// wifi remote events
    // EVENT_WIFI_HM04_POWER,
    EVENT_WIFI_HM04_POWER_ON,
    EVENT_WIFI_HM04_POWER_OFF,
    // EVENT_WIFI_LAMP_POWER,
    EVENT_WIFI_LAMP_POWER_ON,
    EVENT_WIFI_LAMP_POWER_OFF,
    EVENT_WIFI_LAMP_MODE,
    EVENT_WIFI_LAMP_STATIC_COLOR_R,
    EVENT_WIFI_LAMP_STATIC_COLOR_G,
    EVENT_WIFI_LAMP_STATIC_COLOR_B,
    EVENT_WIFI_LAMP_BRIGHTNESS,
    EVENT_WIFI_LAMP_SCENARIO,
    // EVENT_WIFI_MIST_POWER,
    EVENT_WIFI_MIST_POWER_ON,
    EVENT_WIFI_MIST_POWER_OFF,
    EVENT_WIFI_MIST_MODE,
    EVENT_WIFI_SPEAKER_PLAY_PAUSE,
    EVENT_WIFI_SPEAKER_VOLUME,

    EVENT_MAX,

    
} event_t;

void StartFSM(void);


#endif

