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



void test_fsm_door_checkingFsmInitFillsStructure(void)

{



    printf("Test 0\n");

    struct_fsm_door_checking zero;

    printf("Test 1dd\n");

    struct_fsm_door_checking full;

    printf("Test 1\n");

    bzero(&zero, sizeof(struct_fsm_door_checking));

 bzero(&full, sizeof(struct_fsm_door_checking));

 printf("Test 2\n");

   door_fsm_init(&full, 0, 0, 0);

   printf("Test 3\n");

 do {if ((memcmp(&zero, &full, sizeof(struct_fsm_door_checking)) != 0)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(30)));}} while(0);

}
