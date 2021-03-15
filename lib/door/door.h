#ifndef DOOR_H
#define DOOR_H

#include <stdio.h>
#include "stdlib.h"
#include "stdint.h"
#include "hardware_esp8266.h"

#define NDoors 4

void set_active (uint8_t index, uint8_t val);
uint8_t is_active (uint8_t index);

uint8_t get_flag_door_open (uint8_t index);
void set_flag_door_open (uint8_t index, uint8_t val);

uint8_t is_door_open (uint8_t index);

uint8_t get_new_index ();

void set_pin (uint8_t index, uint8_t pin);

#endif