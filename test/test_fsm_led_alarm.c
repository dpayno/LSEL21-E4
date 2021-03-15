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

//TEST_1: Tiene una función de inicialización que rellena una estructura
void test_fsm_led_alarm_FsmInitFillsStructureWithSomething(void)
{
    fsm_led_alarm_t zero;
    fsm_led_alarm_t f;
    
    bzero(&zero, sizeof(fsm_led_alarm_t));
    bzero(&f, sizeof(fsm_led_alarm_t));

    fsm_led_alarm_init(&f, NULL);

    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_led_alarm_t)) != 0);
}

//TEST_2: Compruebo que estoy en el estado inicial cuando empiezo
void test_fsm_led_alarm_FsmFireInitsInIdleState(void)
{
    fsm_led_alarm_t f;

    fsm_led_alarm_init(&f, NULL);

    TEST_ASSERT(f.fsm.current_state == LED_ALARM_IDLE);
}

//TEST_3: Si el estado es IDLE, hay fsm_fire llama a la función is_active() y me devuelve TRUE, cambio a ACTIVE y se actualiza el timeout
void test_fsm_led_alarm_FsmFireFollowsTransitionWhenIdleAndIsActiveIsTrue(void)
{
    fsm_led_alarm_t f; //creo la máquina de estados
    uint32_t flag_active;//creamos la variable flag_active que es la que esta dentro de IsActive()

    get_tick_count_IgnoreAndReturn(10);//para indicarle que nos da igual las veces que se ejecute esa funcion

    fsm_led_alarm_init(&f, &flag_active);//inicializo la FSM y le paso los argumentos a la máquina de estados

    flag_active = 1;//Digo q flag active sea TRUE

    fsm_fire((fsm_t*)&f);//hago un fire para que se ejecute la transición

    TEST_ASSERT(f.fsm.current_state == LED_ALARM_ACTIVE);//le digo al test donde debería estar

    TEST_ASSERT_EQUAL(10+5, f.next_timeout);//Mira que los dos argumetnos son iguales


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

// Test del comportamiento global, varios ciclos de FSM Fire
void test_fsm_led_alarm_CheckBehaviourForSeveralCycles(void)
{
    fsm_led_alarm_t f;
    uint32_t flag_active = 0;

    fsm_led_alarm_init(&f, &flag_active);    
    TEST_ASSERT(f.fsm.current_state == LED_ALARM_IDLE);

    fsm_fire((fsm_t*)&f);
    TEST_ASSERT(f.fsm.current_state == LED_ALARM_IDLE);

    flag_active = 1;
    get_tick_count_ExpectAndReturn(10);
    fsm_fire((fsm_t*)&f);
    TEST_ASSERT_EQUAL(10+5, f.next_timeout);
    TEST_ASSERT(f.fsm.current_state == LED_ALARM_ACTIVE);

    get_tick_count_IgnoreAndReturn(14);
    leds_blink_Ignore();
    fsm_fire((fsm_t*)&f);
    TEST_ASSERT(f.fsm.current_state == LED_ALARM_ACTIVE);

    get_tick_count_ExpectAndReturn(15);
    leds_blink_Expect();
    fsm_fire((fsm_t*)&f);
    TEST_ASSERT_EQUAL(15+5, f.next_timeout);
    TEST_ASSERT(f.fsm.current_state == LED_ALARM_ACTIVE);

    get_tick_count_ExpectAndReturn(21);
    leds_blink_Expect();
    fsm_fire((fsm_t*)&f);
    TEST_ASSERT_EQUAL(20+5, f.next_timeout);
    TEST_ASSERT(f.fsm.current_state == LED_ALARM_ACTIVE);

    flag_active = 0;
    get_tick_count_IgnoreAndReturn(24);
    leds_blink_Ignore();
    leds_off_Expect();
    fsm_fire((fsm_t*)&f);
    TEST_ASSERT(f.fsm.current_state == LED_ALARM_IDLE);
    TEST_ASSERT_EQUAL(0, f.next_timeout);

}