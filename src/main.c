#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "fsm.h"

#include "fsm_door_checking.h"

int active = 0;
int door_opened = 0;
int flag_door_open = 0;


int main ()

{
  fsm_t* door_fsm = door_fsm_init();
  
  while (scanf("%d %d", &active, &door_opened)==2) {
       
      fsm_fire (door_fsm);         
  }
  return 1;
}
