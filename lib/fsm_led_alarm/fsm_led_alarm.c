#include "fsm_led_alarm.h"
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "timer.h"

/* Definición funciones privadas *********************************************/
// Funciones de guarda
static int  is_flag_active (fsm_t* this);
static int  is_not_flag_active (fsm_t* this);
static int timer_finished (fsm_t* this);
// Funciones de salida
static void turn_led_off (fsm_t* this);
static void init_timer (fsm_t* this);
static void blink_leds_and_new_timeout (fsm_t* this);

/* Tabla de transiciones *****************************************************/
static fsm_trans_t led_alarm[] = {
  { LED_ALARM_IDLE,   is_flag_active,     LED_ALARM_ACTIVE, init_timer                 },
  { LED_ALARM_ACTIVE, is_not_flag_active, LED_ALARM_IDLE,   turn_led_off               },
  { LED_ALARM_ACTIVE, timer_finished,     LED_ALARM_ACTIVE, blink_leds_and_new_timeout },
  { -1,               NULL,               -1,               NULL                       },
};

/* Implementación funciones privadas *****************************************/
static int  is_flag_active (fsm_t* this)
{
    fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
    return led_is_active(f->u8_index);
}

static int  is_not_flag_active (fsm_t* this)
{
    fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
    return !led_is_active(f->u8_index);
}

static int timer_finished (fsm_t* this)
{
  fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
  return timer_get_tick_count() >= led_get_current_timeout(f->u8_index);
}

static void turn_led_off (fsm_t* this)
{
  fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
  led_off(f->u8_index);
  led_set_new_timeout(f->u8_index, 0);
}

static void init_timer (fsm_t* this)
{
  fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
  led_set_new_timeout(f->u8_index, timer_get_tick_count() + led_get_blink_time(f->u8_index) );
}

static void blink_leds_and_new_timeout (fsm_t* this)
{
  fsm_led_alarm_t* f = (fsm_led_alarm_t*)this;
  led_blink(f->u8_index);
  led_set_new_timeout(f->u8_index, led_get_current_timeout(f->u8_index) + led_get_blink_time(f->u8_index) );
}

/* Funciones públicas ********************************************************/
void fsm_led_alarm_init(fsm_led_alarm_t* this, uint8_t pin, uint32_t blink_time)
{
  fsm_init ((fsm_t*)this, led_alarm);
  this->u8_index = led_get_new_index();
  led_set_pin (this->u8_index, pin);
  led_set_blink_time (this->u8_index, timer_ms_to_ticks(blink_time));
}

void fsm_led_alarm_turn_on_off_active(fsm_led_alarm_t* this, uint8_t status)
{
  led_set_active (this->u8_index, status);
}