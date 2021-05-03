#ifndef FSM_LED_ALARM_H
#define FSM_LED_ALARM_H

#include "fsm.h"
#include <stdint.h>
#include "led.h"

enum led_alarm_state {
  LED_ALARM_IDLE,
  LED_ALARM_ACTIVE,
};

typedef struct {
  fsm_t fsm_led_alarm;
  uint8_t u8_index;
} fsm_led_alarm_t;

void fsm_led_alarm_init(fsm_led_alarm_t*, uint8_t, uint32_t);
void fsm_led_alarm_turn_on_off_active(fsm_led_alarm_t*, uint8_t);

#endif // FSM_LED_ALARM_H
