#include "build/temp/_test_fsm_door_checking.c"
#include "src/fsm_door_checking.h"
#include "src/fsm.h"
#include "src/fsm_door_checking_test.h"
#include "/usr/local/bundle/gems/ceedling-0.30.0/vendor/unity/src/unity.h"




void setUp(void)

{

}



void tearDown(void)

{

}



void test_fsm_door_checking_fsm_init_fills_structure(void)

{

 UnityIgnore( (("Need to Implement fsm_door_checking")), (UNITY_UINT)(18));

    fsm_t* zero;

    fsm_t* full;

    bzero(zero, sizeof(fsm_t));

 bzero(full, sizeof(fsm_t));



 full = door_fsm_init();



}
