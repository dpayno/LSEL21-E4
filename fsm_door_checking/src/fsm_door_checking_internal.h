#ifndef FSM_DOOR_CHECKING_INTERNAL_H
#define FSM_DOOR_CHECKING_INTERNAL_H

#include "fsm.h"
#include "door.h"

int is_active_and_door_open(fsm_t*);

int is_not_active (fsm_t* this);

void set_door_flag (fsm_t* this);

void clear_door_flag (fsm_t* this);

#endif