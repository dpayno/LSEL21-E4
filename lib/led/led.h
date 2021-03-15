#ifndef _LED_H
#define _LED_H

#include <stdint.h>

#define NLeds 2

uint8_t get_new_index ();

void set_pin (uint8_t index, uint8_t pin);

void set_active (uint8_t index, uint8_t val);
uint8_t is_active (uint8_t index);

void set_new_timeout (uint8_t index, uint32_t new_timeout);
uint32_t get_current_timeout (uint8_t index);

void set_blink_time (uint8_t index, uint16_t blink_time);
uint16_t get_blink_time (uint8_t index);

void led_off (uint8_t index);
void led_blink (uint8_t index);

#endif // _LED_H