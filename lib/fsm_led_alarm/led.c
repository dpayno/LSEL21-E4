#include "led.h"
#include "hardware_esp8266.h"

typedef union {
  struct
  {
    unsigned active:1;
    unsigned led_status:1;
    unsigned pin:5;
  };
  uint8_t flags_byte;
}flags_led_t;

typedef struct 
{
  flags_led_t flags_led;
  uint32_t next_timeout;
  uint16_t blink_in_ticks;
}flags_and_data_leds_t;


static flags_and_data_leds_t data_leds[N_LEDS];
static uint8_t m_u8_index = 0;

uint8_t led_get_new_index ()
{
  data_leds[m_u8_index].flags_led.flags_byte = 0x00;
  data_leds[m_u8_index].next_timeout = 0;
  return m_u8_index++;
}

void led_set_pin (uint8_t index, uint8_t pin)
{
  hard_gpio_pinSetUp (pin, HARD_GPIO_OUTPUT);
  hard_gpio_digitalWrite (pin, 0);
  data_leds[index].flags_led.pin = pin;
  data_leds[index].flags_led.led_status = 0;
}

void led_set_active (uint8_t index, uint8_t val)
{
  data_leds[index].flags_led.active = val;
}

uint8_t led_is_active (uint8_t index)
{
  return data_leds[index].flags_led.active;
}

void led_set_new_timeout (uint8_t index, uint32_t new_timeout)
{
  data_leds[index].next_timeout = new_timeout;
}

uint32_t led_get_current_timeout (uint8_t index)
{
  return data_leds[index].next_timeout;
}

void led_set_blink_time (uint8_t index, uint16_t blink_time)
{
  data_leds[index].blink_in_ticks = blink_time;
}

uint16_t led_get_blink_time (uint8_t index)
{
  return data_leds[index].blink_in_ticks;
}

void led_off (uint8_t index)
{
  hard_gpio_digitalWrite (data_leds[index].flags_led.pin, 0);
  data_leds[index].flags_led.led_status = 0;

}
void led_blink (uint8_t index)
{
  data_leds[index].flags_led.led_status = !data_leds[index].flags_led.led_status;
  hard_gpio_digitalWrite (data_leds[index].flags_led.pin, data_leds[index].flags_led.led_status);
}