//#include <assert.h>
#include <stdio.h>
//#include <unistd.h>
//#include <signal.h>
//#include "fsm.h"

#include "fsm_door_checking.h"

/*int active = 0;
int door_opened = 0;
int flag_door_open = 0;*/

/*void init_structure (struct_fsm_door_checking* qwerty)
{
	qwerty->u8_active = 0;
	qwerty->u8_door_opened = 0;
	qwerty->u8_flag_door_open = 0;
	door_fsm_init_2(qwerty->fsm_door_checking);
}*/

int main ()

{
  //fsm_t* door_fsm = door_fsm_init();
  struct_fsm_door_checking * door_fsm;
  door_fsm_init(door_fsm);
  
  while (1) {
       
      fsm_fire((fsm_t*)door_fsm);         
  }
  return 1;
}
