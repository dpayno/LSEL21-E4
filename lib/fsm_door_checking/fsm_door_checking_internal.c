#include "fsm_door_checking_internal.h"

int is_active_and_door_open (fsm_t* this)
{
	fsm_door_checking_t * fp = (fsm_door_checking_t*)this;
	return ( is_active(fp->u8_index) && is_door_open(fp->u8_index) ); 
}
int is_not_active (fsm_t* this) 
{
	fsm_door_checking_t * fp = (fsm_door_checking_t*)this;
	return ! is_active(fp->u8_index); 
}

// Funciones de salida

void set_door_flag (fsm_t* this)
{
	fsm_door_checking_t * fp = (fsm_door_checking_t*)this;
  	set_flag_door_open (fp->u8_index, 1);
  
}

void clear_door_flag (fsm_t* this)
{
	fsm_door_checking_t * fp = (fsm_door_checking_t*)this;
  	set_flag_door_open (fp->u8_index, 0);
}