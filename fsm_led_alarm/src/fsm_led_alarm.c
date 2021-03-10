#include "fsm_led_alarm.h"
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include "timer.h"
#include "leds.h"

#define BLINK_TIME 5

//-------funciones de guarda para realizar las transiciones y delcaracion de entradas-----------
static int  is_active (fsm_t* this)
{
    fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
    if (f->flag_active) {
        return *(f->flag_active);
    } else {
        return 0;
    }
}

static int  is_not_active (fsm_t* this)
{
    fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
    if (f->flag_active) {
        return !*(f->flag_active);
    } else {
        return 0;
    }
}

static int timer_finished (fsm_t* this)
{
  fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
  return get_tick_count() >= f->next_timeout;
}

//-------------------Funciones de salida-----------------------------------------------  
static void led_off (fsm_t* this)
{
  printf("Led Off!!!\n");
  leds_off();
  fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
  f->next_timeout = 0;
}

static void init_timer (fsm_t* this)
{
  fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
  f->next_timeout = get_tick_count() + BLINK_TIME;
}

static void blink_leds_and_new_timeout (fsm_t* this)
{
  printf("Blink Led...\n");
  leds_blink();
  fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
  f->next_timeout += BLINK_TIME;
}


// Explicit FSM description
//------------Tabla de transiciones de la FSM con logica de salida------------------------------
static fsm_trans_t led_alarm[] = {

  { LED_ALARM_IDLE, is_active, LED_ALARM_ACTIVE, init_timer },
  { LED_ALARM_ACTIVE, is_not_active, LED_ALARM_IDLE, led_off },
  { LED_ALARM_ACTIVE, timer_finished, LED_ALARM_ACTIVE, blink_leds_and_new_timeout },
  {-1, NULL, -1, NULL },

};

// Public function

void fsm_led_alarm_init(fsm_led_alarm_t* f, uint32_t* flag_active)
{
  f->flag_active = flag_active;
  fsm_init((fsm_t*)f, led_alarm);
}
