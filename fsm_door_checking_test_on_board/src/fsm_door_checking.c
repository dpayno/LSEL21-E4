#include "fsm_door_checking.h"

#include "door.h"
#include "fsm_door_checking_internal.h"

// Explicit FSM description

static fsm_trans_t door_trans[] = {

  	{ DISABLE,  is_active_and_door_open,  ENABLE,   set_door_flag   },
  	{ ENABLE ,  is_not_active,            DISABLE,  clear_door_flag },
  	{-1, NULL, -1, NULL },
};


// Public function

void fsm_door_checking_init(fsm_door_checking_t* this, uint8_t pin)
{
	//printf("init_starting\n");

	fsm_init((fsm_t*)this, door_trans);
	this->u8_index = get_new_index();
	set_pin (this->u8_index, pin);

	//printf("init complete\n");
}

uint8_t fsm_door_checking_is_in_alarm (fsm_door_checking_t* this)
{
	return get_flag_door_open (this->u8_index);
}

void fsm_door_checking_turn_on_off_active (fsm_door_checking_t* this, uint8_t status)
{
	set_active (this->u8_index, status);
}