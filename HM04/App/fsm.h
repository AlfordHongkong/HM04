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
    EVENT_PAIR_KEY_SHORT = 1,
    EVENT_PAIR_KEY_LONG,
    EVENT_LAMP_KEY_SHORT,
    EVENT_LAMP_KEY_LONG,
    EVENT_MIST_KEY_SHORT,
    EVENT_MIST_KEY_LONG,
    
    BUTTON_FAN_SHORT,
    BUTTON_FAN_LONG,
    EVENT_CLEANING_ACC,
    EVENT_TIMER_SETTING_ACC,
    EVENT_TIMER_ACC,
    EVENT_FAULT,
    EVENT_TURBO_TEST,
    EVENT_START_PAIRING,
    EVENT_STOP_PAIRING,
    EVENT_MAX,
} event_t;

void StartFSM(void);


#endif

