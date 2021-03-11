#include "build/temp/_test_fsm_door_checking.c"
#include "build/test/mocks/mock_fsm_door_checking_internal.h"
#include "src/door.h"
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



    fsm_door_checking_t zero;

    fsm_door_checking_t full;



    bzero(&zero, sizeof(fsm_door_checking_t));

   bzero(&full, sizeof(fsm_door_checking_t));



   fsm_door_checking_init(&full, 2);



   do {if ((memcmp(&zero, &full, sizeof(fsm_door_checking_t)) != 0)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(33)));}} while(0);

}



void test_fsm_door_checking_FireCalls_is_active_and_door_open_WhenDisable(void)

{

    fsm_door_checking_t f;



    is_active_and_door_open_CMockExpectAnyArgsAndReturn(40, 0);



    fsm_door_checking_init(&f, 8);



    do {if ((f.fsm_door_checking.current_state == DISABLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(44)));}} while(0);



    fsm_fire((fsm_t*)&f);

}





void test_fsm_door_checking_FireLeadsTransitionWhenDisable_and_is_active_and_door_openIsTrue(void)

{

    fsm_door_checking_t f;



    is_active_and_door_open_CMockIgnoreAndReturn(54, 1);



    fsm_door_checking_init(&f, 8);

    fsm_fire((fsm_t*)&f);



    do {if ((f.fsm_door_checking.current_state == ENABLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(59)));}} while(0);

}



void test_fsm_door_checking_FireDontLeadsTransitionWhenDisable_and_is_active_and_door_openIsFalse(void)

{

    fsm_door_checking_t f;



    is_active_and_door_open_CMockIgnoreAndReturn(66, 0);



    fsm_door_checking_init(&f, 8);

    fsm_fire((fsm_t*)&f);



    do {if ((f.fsm_door_checking.current_state == DISABLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(71)));}} while(0);

}











void test_fsm_door_checking_FireCalls_is_not_active_WhenEnable(void)

{

    fsm_door_checking_t f;



    is_not_active_CMockExpectAnyArgsAndReturn(82, 1);



    fsm_door_checking_init(&f, 8);

    f.fsm_door_checking.current_state = ENABLE;



    fsm_fire((fsm_t*)&f);

}



void test_fsm_door_checking_FireLeadsTransitionWhenEnable_and_is_not_active_IsTrue(void)

{

    fsm_door_checking_t f;



    is_not_active_CMockIgnoreAndReturn(94, 1);



    fsm_door_checking_init(&f, 8);

    f.fsm_door_checking.current_state = ENABLE;

    fsm_fire((fsm_t*)&f);



    do {if ((f.fsm_door_checking.current_state == DISABLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(100)));}} while(0);

}



void test_fsm_door_checking_FireDontLeadsTransitionWhenEnable_and_is_not_active_IsFalse(void)

{

    fsm_door_checking_t f;



    is_not_active_CMockIgnoreAndReturn(107, 0);



    fsm_door_checking_init(&f, 8);

    f.fsm_door_checking.current_state = ENABLE;

    fsm_fire((fsm_t*)&f);



    do {if ((f.fsm_door_checking.current_state == ENABLE)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(113)));}} while(0);

}
