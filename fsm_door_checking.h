#include "fsm.h"

enum door_checking_states {

  DISABLE,
  ENABLE,
};

typedef struct 
{
	fsm_t fsm_door_checking;
	uint8_t u8_active;
	uint8_t u8_door_opened;
	uint8_t u8_flag_door_open;
}struct_fsm_door_checking;

void door_fsm_init(struct_fsm_door_checking* this);
