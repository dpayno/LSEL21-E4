#include "unity.h"
#include "string.h"
#include "fsm_door_checking_test.h"
#include "fsm.h"
#include "fsm_door_checking.h"


void setUp(void)
{
}

void tearDown(void)
{
}

void test_fsm_door_checking_fsm_init_fills_structure(void)
{
	//TEST_IGNORE_MESSAGE("Need to Implement fsm_door_checking");  //Emplear cuando se quiere ignaorar un test
    fsm_t* zero;
    fsm_t* full;
    bzero(zero, sizeof(fsm_t));
	bzero(full, sizeof(fsm_t));

	full = door_fsm_init();
	TEST_ASSERT(memcmp(zero, full, sizeof(fsm_t)) != 0);
}

//TEST_IGNORE_MESSAGE("Need to Implement fsm_door_checking");  Emplear cuando se quiere ignaorar un test

