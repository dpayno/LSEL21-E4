#include "fsm_door_checking.h"

#include <stdio.h>
#include <stdlib.h>
#include "door.h"

/* Definición funciones privadas *********************************************/
// Funciones de guarda
static int is_active_and_door_open(fsm_t*);
static int is_not_active (fsm_t*);
// Funciones de salida
static void set_door_flag (fsm_t*);
static void clear_door_flag (fsm_t*);

/* Tabla de transiciones *****************************************************/
static fsm_trans_t door_trans[] = {
	{ DISABLE,  is_active_and_door_open,  ENABLE,   set_door_flag   },
	{ ENABLE ,  is_not_active,            DISABLE,  clear_door_flag },
	{-1, NULL, -1, NULL },
};

/* Implementación funciones privadas *****************************************/
static int is_active_and_door_open (fsm_t* this)
{
	fsm_door_checking_t * fp = (fsm_door_checking_t*)this;
	return ( is_active(fp->u8_index) && is_door_open(fp->u8_index) ); 
}

static int is_not_active (fsm_t* this) 
{
	fsm_door_checking_t * fp = (fsm_door_checking_t*)this;
	return ! is_active(fp->u8_index); 
}

static void set_door_flag (fsm_t* this)
{
	fsm_door_checking_t * fp = (fsm_door_checking_t*)this;
  	set_flag_door_open (fp->u8_index, 1);
  
}

static void clear_door_flag (fsm_t* this)
{
	fsm_door_checking_t * fp = (fsm_door_checking_t*)this;
  	set_flag_door_open (fp->u8_index, 0);
}

/* Funciones públicas ********************************************************/
void fsm_door_checking_init(fsm_door_checking_t* this, uint8_t pin)
{
	fsm_init((fsm_t*)this, door_trans);
	this->u8_index = door_get_new_index();
	door_set_pin (this->u8_index, pin);
}

uint8_t fsm_door_checking_is_in_alarm (fsm_door_checking_t* this)
{
	return door_get_flag_door_open (this->u8_index);
}

void fsm_door_checking_turn_on_off_active (fsm_door_checking_t* this, uint8_t status)
{
	door_set_active (this->u8_index, status);
}