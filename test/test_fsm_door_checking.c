#include "unity.h"
#include "string.h"
#include "fsm.h"
#include "fsm_door_checking.h"
#include <stdio.h>
#include "stdlib.h"
#include "stdint.h"
#include "door.h"

#include "mock_fsm_door_checking_internal.h"


void setUp(void)
{
}

void tearDown(void)
{
}

void test_fsm_door_checkingFsmInitFillsStructure(void)
{
	//TEST_IGNORE_MESSAGE("Need to Implement fsm_door_checking");  //Emplear cuando se quiere ignaorar un test
    fsm_door_checking_t  zero;
    fsm_door_checking_t  full;

    bzero(&zero, sizeof(fsm_door_checking_t));
	  bzero(&full, sizeof(fsm_door_checking_t));

  	fsm_door_checking_init(&full, 2);

	  TEST_ASSERT(memcmp(&zero, &full, sizeof(fsm_door_checking_t)) != 0);
}

void test_fsm_door_checking_FireCalls_is_active_and_door_open_WhenDisable(void)
{
    fsm_door_checking_t f;

    is_active_and_door_open_ExpectAnyArgsAndReturn(0);

    fsm_door_checking_init(&f, 8);

    TEST_ASSERT(f.fsm_door_checking.current_state == DISABLE);

    fsm_fire((fsm_t*)&f);
}


void test_fsm_door_checking_FireLeadsTransitionWhenDisable_and_is_active_and_door_openIsTrue(void)
{
    fsm_door_checking_t f;

    is_active_and_door_open_IgnoreAndReturn(1);
    set_door_flag_Expect();

    fsm_door_checking_init(&f, 8);
    fsm_fire((fsm_t*)&f);

    TEST_ASSERT(f.fsm_door_checking.current_state == ENABLE);
}

void test_fsm_door_checking_FireDontLeadsTransitionWhenDisable_and_is_active_and_door_openIsFalse(void)
{
    fsm_door_checking_t f;

    is_active_and_door_open_IgnoreAndReturn(0);

    fsm_door_checking_init(&f, 8);
    fsm_fire((fsm_t*)&f);

    TEST_ASSERT(f.fsm_door_checking.current_state == DISABLE);
}





void test_fsm_door_checking_FireCalls_is_not_active_WhenEnable(void)
{
    fsm_door_checking_t f;

    is_not_active_ExpectAnyArgsAndReturn(1);
    clear_door_flag_Expect();

    fsm_door_checking_init(&f, 8);
    f.fsm_door_checking.current_state = ENABLE;

    fsm_fire((fsm_t*)&f);
}

void test_fsm_door_checking_FireLeadsTransitionWhenEnable_and_is_not_active_IsTrue(void)
{
    fsm_door_checking_t f;

    is_not_active_IgnoreAndReturn(1);
    clear_door_flag_Expect();

    fsm_door_checking_init(&f, 8);
    f.fsm_door_checking.current_state = ENABLE;
    fsm_fire((fsm_t*)&f);

    TEST_ASSERT(f.fsm_door_checking.current_state == DISABLE);
}

void test_fsm_door_checking_FireDontLeadsTransitionWhenEnable_and_is_not_active_IsFalse(void)
{
    fsm_door_checking_t f;

    is_not_active_IgnoreAndReturn(0);

    fsm_door_checking_init(&f, 8);
    f.fsm_door_checking.current_state = ENABLE;
    fsm_fire((fsm_t*)&f);

    TEST_ASSERT(f.fsm_door_checking.current_state == ENABLE);
}


//TEST_IGNORE_MESSAGE("Need to Implement fsm_door_checking");  Emplear cuando se quiere ignaorar un test

