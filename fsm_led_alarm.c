
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

#include "fsm.h"
#include "fsm_led_alarm.h"

extern int  active;
extern int timer;
extern int led_state;

#define BLINK_TIME 500

//-------funciones de guarda para realizar las transiciones y delcaracion de entradas-----------
static int  is_active (fsm_t* this) { return active;}
static int  is_not_active (fsm_t* this) { return !active;}

static void timer_isr (union sigval arg) { timer = 1; }
static void timer_start (int ms)

{
  timer_t timerid;
  struct itimerspec spec;
  struct sigevent se;
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_value.sival_ptr = &timerid;
  se.sigev_notify_function = timer_isr;
  se.sigev_notify_attributes = NULL;
  spec.it_value.tv_sec = ms / 1000;
  spec.it_value.tv_nsec = (ms % 1000) * 1000000;
  spec.it_interval.tv_sec = 0;
  spec.it_interval.tv_nsec = 0;
  timer_create (CLOCK_REALTIME, &se, &timerid);
  timer_settime (timerid, 0, &spec, NULL);
}
static int timer_finished (fsm_t* this) { return timer; }
//-------------------Funciones de salida-----------------------------------------------  
static void led_off (fsm_t* this)

{
  printf("Led Off!!!\n");
  led_state = 0;
}


static void init_timer (fsm_t* this)

{
  timer=0;
  timer_start(BLINK_TIME);
}

static void blink_leds_and_new_timeout (fsm_t* this)

{
  printf("Blink Led...\n");
  led_state = !led_state;
  timer=0;
  timer_start(BLINK_TIME);
}


// Explicit FSM description
//------------Tabla de transiciones de la FSM con logica de salida------------------------------
static fsm_trans_t led_alarm[] = {

  { led_alarm_idle, is_active, led_alarm_active, init_timer }, 
  { led_alarm_active, timer_finished, led_alarm_active, blink_leds_and_new_timeout }, 
  { led_alarm_active, is_not_active, led_alarm_idle, led_off },   
  {-1, NULL, -1, NULL },

};

// Public function

fsm_t* fsm_led_alarm_init()
{
  return fsm_new (led_alarm);
}
