#include "fsm.h"

enum door_checking_states {

  DISABLE,
  ENABLE,
};


struct _struct_fsm_door_checking {
	fsm_t fsm_door_checking;
	int* u8_active;
	int* u8_door_opened;
	int* u8_flag_door_open;
};
typedef struct _struct_fsm_door_checking struct_fsm_door_checking;


void door_fsm_init(struct_fsm_door_checking*, int, int, int);

/*
typedef struct
{
fsm_t fsm_door_checking;
uint8_t* u8_active;
uint8_t* u8_door_opened;
uint8_t* u8_flag_door_open;
}struct_fsm_door_checking;
*/