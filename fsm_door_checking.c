#include <stdlib.h>
#include <stdio.h>
//#include "fsm.h"
#include "fsm_door_checking.h"


// Funciones guarda
static int is_active_and_door_open (fsm_t* this)
{
	struct_fsm_door_checking * fp = (struct_fsm_door_checking*)this;
	return (fp->u8_active == 1 && fp->u8_door_opened == 1); 
}
static int is_not_active (fsm_t* this) 
{
	struct_fsm_door_checking * fp = (struct_fsm_door_checking*)this;
	return !(fp->u8_active); 
}


// Funciones de salida

static void set_door_flag (fsm_t* this)
{
	struct_fsm_door_checking * fp = (struct_fsm_door_checking*)this;
  	fp->u8_flag_door_open = 1;
 	printf("PUERTA ABIERTA...: flag = %d \n", fp->u8_flag_door_open);
  
}

static void clear_door_flag (fsm_t* this)
{
	struct_fsm_door_checking * fp = (struct_fsm_door_checking*)this;
  	fp->u8_flag_door_open = 0;
  	printf("ALARMA APAGADA...flag = %d \n", fp->u8_flag_door_open);
}


// Explicit FSM description

static fsm_trans_t door_trans[] = {

  	{ DISABLE,  is_active_and_door_open,  ENABLE,   set_door_flag   },
  	{ ENABLE ,  is_not_active,            DISABLE,  clear_door_flag },
  	{-1, NULL, -1, NULL },
};


// Public function

void door_fsm_init(struct_fsm_door_checking* this)
{
	printf("init_starting");
	fsm_init((fsm_t*)this, door_trans);
	this->u8_active = 0;
	this->u8_door_opened = 0;
	this->u8_flag_door_open = 0;
	printf("init complete");
}

/*void door_fsm_init_2(fsm_t * this)
{
	fsm_init(this, door_trans);
}*/
