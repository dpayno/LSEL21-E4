#include "unity.h"

#include <string.h>

#include "fsm_led_alarm.h"
#include "fsm.h"
#include "mock_timer.h"
#include "mock_leds.h"

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

void test_fsm_led_alarm_FsmFireInitsInIdleState(void)
{
    fsm_led_alarm_t f;

    fsm_led_alarm_init(&f, NULL);

    TEST_ASSERT(f.fsm.current_state == LED_ALARM_IDLE);
}

void test_fsm_led_alarm_FsmFireFollowsTransitionWhenIdleAndIsActiveIsTrue(void)
{
    fsm_led_alarm_t f;
    uint32_t flag_active;

    get_tick_count_IgnoreAndReturn(0);

    fsm_led_alarm_init(&f, &flag_active);

    flag_active = 1;

    fsm_fire((fsm_t*)&f);

    TEST_ASSERT(f.fsm.current_state == LED_ALARM_ACTIVE);
}