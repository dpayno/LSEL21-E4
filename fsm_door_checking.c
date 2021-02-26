
#include "fsm.h"
#include "fsm_door_checking.h"

// Flags
extern int active;
extern int door_opened;
extern int flag_door_open;

// Funciones guarda
static int is_active_and_door_open (fsm_t* this) { return (active == 1 && door_opened == 1); }
static int is_not_active (fsm_t* this) { return !active; }


// Funciones de salida

static void set_door_flag (fsm_t* this)
{
  flag_door_open = 1;
  printf("PUERTA ABIERTA...: flag = %d \n", flag_door_open);
  
}

static void clear_door_flag (fsm_t* this)
{
  flag_door_open = 0;
  printf("ALARMA APAGADA...flag = %d \n", flag_door_open);
}


// Explicit FSM description

static fsm_trans_t door_trans[] = {

  { DISABLE,  is_active_and_door_open,  ENABLE,   set_door_flag   },
  { ENABLE ,  is_not_active,            DISABLE,  clear_door_flag },
  {-1, NULL, -1, NULL },

};


// Public function

fsm_t* door_fsm_init()
{
	return fsm_new (door_trans);
}
