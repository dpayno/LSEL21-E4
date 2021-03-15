#ifndef FSM_DOOR_CHECKING_INTERNAL_H
#define FSM_DOOR_CHECKING_INTERNAL_H

#include "fsm.h"
#include "fsm_door_checking.h"
#include "door.h"

int is_active_and_door_open(fsm_t*);

int is_not_active (fsm_t*);

void set_door_flag (fsm_t*);

void clear_door_flag (fsm_t*);

#endif