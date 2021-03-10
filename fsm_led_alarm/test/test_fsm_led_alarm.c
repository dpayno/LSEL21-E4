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

    // Comprobamos que la funcion fsm_led_alarm_init ocupa espacio en memoriaWS
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



