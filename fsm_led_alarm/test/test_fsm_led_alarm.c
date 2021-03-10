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

// Tiene una función de inicialización que rellena una estructura
void test_fsm_led_alarm_FsmInitFillsStructureWithSomething(void)
{
    fsm_led_alarm_t zero;
    fsm_led_alarm_t f;
    
    bzero(&zero, sizeof(fsm_led_alarm_t));
    bzero(&f, sizeof(fsm_led_alarm_t));

    fsm_led_alarm_init(&f, NULL);

    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_led_alarm_t)) != 0);
}

// La máquina de estados se inicializa en estado IDLE
void test_fsm_led_alarm_FsmFireInitsInIdleState(void)
{
    fsm_led_alarm_t f;

    fsm_led_alarm_init(&f, NULL);

    TEST_ASSERT(f.fsm.current_state == LED_ALARM_IDLE);
}

// Si, estando en IDLE, la función is_active() devuelve TRUE, se cambia de estado a ACTIVE.
void test_fsm_led_alarm_FsmFireFollowsTransitionWhenIdleAndFlagActiveIsTrue(void)
{
    fsm_led_alarm_t f;
    uint32_t flag_active;

    get_tick_count_IgnoreAndReturn(0);

    fsm_led_alarm_init(&f, &flag_active);

    flag_active = 1;

    fsm_fire((fsm_t*)&f);

    TEST_ASSERT(f.fsm.current_state == LED_ALARM_ACTIVE);
}


// Si, estando en ACTIVE, el flag active es FALSE, se ejecuta la función led_off() y se resetea el timeout
// Si, estando en ACTIVE, el flag active es FALSE, se cambia al estado IDLE.
void test_fsm_led_alarm_FsmFireFollowsActiveToIdleAndFsmCallsLedOffWhenFlagActiveIsFalse(void)
{
    fsm_led_alarm_t f;
    uint32_t flag_active;

    fsm_led_alarm_init(&f, &flag_active);
    f.fsm.current_state = LED_ALARM_ACTIVE;
    flag_active = 0;
    
    leds_off_Expect();
    fsm_fire((fsm_t*)&f);

    TEST_ASSERT_EQUAL(0, f.next_timeout);
    TEST_ASSERT(f.fsm.current_state == LED_ALARM_IDLE);
}

// Si, estando en ACTIVE, el flag active es TRUE, se sigue en el estado ACTIVE.
void test_fsm_led_alarm_FsmFireStaysActiveWhenFlagActiveIsTrue(void)
{
    fsm_led_alarm_t f;
    uint32_t flag_active;

    get_tick_count_ExpectAndReturn(0);

    fsm_led_alarm_init(&f, &flag_active);
    f.fsm.current_state = LED_ALARM_ACTIVE;
    flag_active = 1;

    fsm_fire((fsm_t*)&f);

    TEST_ASSERT(f.fsm.current_state == LED_ALARM_ACTIVE);
}

// Si, estando en ACTIVE, la función finish_timer() devuelve TRUE, se ejecuta la función blink_leds() y se actualiza el timeout
void test_fsm_led_alarm_FsmFireCallsBlinkLedsAndRefreshesTimeoutWhenTimerIsFinished(void)
{
    fsm_led_alarm_t f;
    uint32_t flag_active;

    get_tick_count_ExpectAndReturn(10);
    leds_blink_Expect();

    fsm_led_alarm_init(&f, &flag_active);
    f.fsm.current_state = LED_ALARM_ACTIVE;
    f.next_timeout = 0;
    flag_active = 1;

    fsm_fire((fsm_t*)&f);

    TEST_ASSERT_EQUAL(5, f.next_timeout);
}