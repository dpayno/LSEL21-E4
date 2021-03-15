//#include <assert.h>
#include <stdio.h>
//#include <unistd.h>
//#include <signal.h>
//#include "fsm.h"

#include "fsm_door_checking.h"

int main ()

{
  //fsm_t* door_fsm = door_fsm_init();
  fsm_door_checking_t  left_door_fsm;
  fsm_door_checking_t  right_door_fsm;
  int alarm_status = 0;

  //printf("Struct Created\n");

  fsm_door_checking_init(&left_door_fsm, 10);
  fsm_door_checking_init(&right_door_fsm, 2);

  printf("All init done... entering main loop\n");
  
  printf("Signal simulation, is_active?\n");

  while (scanf("%d", &alarm_status) == 1) {

  	fsm_door_checking_turn_on_off_active (&left_door_fsm, alarm_status);
  	fsm_door_checking_turn_on_off_active (&right_door_fsm, alarm_status);

    fsm_fire((fsm_t*)&left_door_fsm);
    fsm_fire((fsm_t*)&right_door_fsm);

    if ( fsm_door_checking_is_in_alarm (&left_door_fsm) )	printf("ALARM %d\n", left_door_fsm.u8_index);
    if ( fsm_door_checking_is_in_alarm (&right_door_fsm) )	printf("ALARM %d\n", right_door_fsm.u8_index);


    else printf("ALL GOOD\n");     
  }
  return 1;
}
