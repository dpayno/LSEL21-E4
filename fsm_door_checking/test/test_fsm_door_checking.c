#include "unity.h"
#include "string.h"
#include "fsm_door_checking_test.h"
#include "fsm.h"
#include "fsm_door_checking.h"
#include <stdio.h>


void setUp(void)
{
}

void tearDown(void)
{
}

void test_fsm_door_checkingFsmInitFillsStructure(void)
{
	//TEST_IGNORE_MESSAGE("Need to Implement fsm_door_checking");  //Emplear cuando se quiere ignaorar un test
    printf("Test 0\n");
    struct_fsm_door_checking  zero;
    printf("Test 1dd\n");
    struct_fsm_door_checking  full;
    printf("Test 1\n");
    bzero(&zero, sizeof(struct_fsm_door_checking));
	bzero(&full, sizeof(struct_fsm_door_checking));
	printf("Test 2\n");
  	door_fsm_init(&full, 0, 0, 0);
  	printf("Test 3\n");
	TEST_ASSERT(memcmp(&zero, &full, sizeof(struct_fsm_door_checking)) != 0);
}

//TEST_IGNORE_MESSAGE("Need to Implement fsm_door_checking");  Emplear cuando se quiere ignaorar un test

