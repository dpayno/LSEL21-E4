#include "unity.h"

#include <string.h>

#include "fsm_led_alarm.h"
#include "fsm.h"
#include "mock_timer.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_fsm_led_alarm_FsmInitFillsStructureWithSomething(void)
{
    fsm_led_alarm_t zero;
    fsm_led_alarm_t f;
    
    bzero(&zero, sizeof(fsm_led_alarm_t));
    bzero(&f, sizeof(fsm_led_alarm_t));

    fsm_led_alarm_init(&f, NULL);

    // Comprobamos que la funcion fsm_led_alarm_init ocupa espacio en memoriaWS
    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_led_alarm_t)) != 0);
}
