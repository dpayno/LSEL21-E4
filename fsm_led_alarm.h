#include "fsm.h"

//--------------------FSM_led_alarm States----------------------------
enum led_alarm_state {
  LED_ALARM_IDLE,
  LED_ALARM_ACTIVE,
};

fsm_t* fsm_led_alarm_init();