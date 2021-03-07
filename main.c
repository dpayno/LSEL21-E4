#include "fsm.h"
#include "fsm_led_alarm.h"

int active = 0;
int timer = 0;
int led_state = 0;

int main ()
{
  fsm_t* FSM_led_alarm = fsm_led_alarm_init();

  while (1) {
      fsm_fire (FSM_led_alarm);
  }
  return 1;
}