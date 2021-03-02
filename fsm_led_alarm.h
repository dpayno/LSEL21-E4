//--------------------FSM_led_alarm States----------------------------
enum led_alarm_state {

  led_alarm_idle,
  led_alarm_active,
};

fsm_t* fsm_led_alarm_init();