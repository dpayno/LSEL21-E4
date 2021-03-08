#ifndef FSM_LED_ALARM_H
#define FSM_LED_ALARM_H

#include "fsm.h"
#include <stdint.h>

//--------------------FSM_led_alarm States----------------------------
enum led_alarm_state {
  LED_ALARM_IDLE,
  LED_ALARM_ACTIVE,
};

struct _fsm_led_alarm_t {
    fsm_t fsm;
    uint32_t* flag_active;
    uint32_t next_timeout;
};
typedef struct _fsm_led_alarm_t fsm_led_alarm_t;


void fsm_led_alarm_init(fsm_led_alarm_t* f, uint32_t* flag_active);

#endif // FSM_LED_ALARM_H
