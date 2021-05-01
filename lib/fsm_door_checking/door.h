#ifndef DOOR_H
#define DOOR_H

#include <stdio.h>
#include "stdlib.h"
#include "stdint.h"
#include "hardware_esp8266.h"

#define N_DOORS 4

typedef union {
  struct {
    unsigned active:1;
    unsigned door_status:1;
    unsigned flag_door_open:1;
    unsigned pin:5;
  };
  uint8_t data;
} flags_door_t;

void door_set_active (uint8_t index, uint8_t val);
uint8_t door_is_active (uint8_t index);

uint8_t door_get_flag_door_open (uint8_t index);
void door_set_flag_door_open (uint8_t index, uint8_t val);

uint8_t door_is_door_open (uint8_t index);

uint8_t door_get_new_index ();

void door_set_pin (uint8_t index, uint8_t pin);

#endif // DOOR_H