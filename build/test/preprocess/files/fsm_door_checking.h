#include "src/fsm.h"












enum door_checking_states {

  DISABLE,

  ENABLE,

};



typedef struct

{

 fsm_t fsm_door_checking;

 uint8_t u8_index;

}fsm_door_checking_t;



void fsm_door_checking_init (fsm_door_checking_t*, uint8_t);



uint8_t fsm_door_checking_is_in_alarm (fsm_door_checking_t*);



void fsm_door_checking_turn_on_off_active (fsm_door_checking_t*, uint8_t);
