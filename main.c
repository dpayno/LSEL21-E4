#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "fsm.h"
#include "fsm_led_alarm.h"


int  active = 1;
int timer = 0;
int led_state = 0;

// res = a - b

void timeval_sub (struct timeval *res, struct timeval *a, struct timeval *b)

{

  res->tv_sec = a->tv_sec - b->tv_sec;
  res->tv_usec = a->tv_usec - b->tv_usec;
  if (res->tv_usec < 0) {
    --res->tv_sec;
    res->tv_usec += 1000000;
  }
}


// res = a + b

void timeval_add (struct timeval *res, struct timeval *a, struct timeval *b)

{

  res->tv_sec = a->tv_sec + b->tv_sec
    + a->tv_usec / 1000000 + b->tv_usec / 1000000; 
  res->tv_usec = a->tv_usec % 1000000 + b->tv_usec % 1000000;

}



// wait until next_activation (absolute time)

void delay_until (struct timeval* next_activation)

{

  struct timeval now, timeout;
  gettimeofday (&now, NULL);
  timeval_sub (&timeout, next_activation, &now);
  select (0, NULL, NULL, NULL, &timeout);

}


int main ()

{

  
  struct timeval clk_period = { 0, 250 * 1000 };
  struct timeval next_activation;

  fsm_t* FSM_led_alarm = fsm_led_alarm_init();

  gettimeofday (&next_activation, NULL);

  while (1)
  {
      fsm_fire (FSM_led_alarm);
      gettimeofday (&next_activation, NULL);
      timeval_add (&next_activation, &next_activation, &clk_period);
      delay_until (&next_activation);    
  }
  return 1;
}