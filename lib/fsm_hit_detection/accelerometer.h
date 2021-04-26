#ifndef ACCELEROMETER__H
#define ACCELEROMETER__H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hardware_esp8266.h"

#define N_ACCEL 1

typedef struct accel_threshold_t {
  uint16_t x;
  uint16_t y;
  uint16_t z;
} accel_threshold_t;

uint8_t get_new_index ();
void accel_set_thresholds ( uint8_t index, accel_threshold_t hard, accel_threshold_t soft);

void accel_set_pins ( uint8_t index, uint8_t pin_SDA, uint8_t pin_SCL );
int8_t accel_get_pins ( uint8_t index, uint8_t* pin_SDA, uint8_t* pin_SCL );

void accel_set_timeout ( uint8_t index, uint16_t new_timeout);
uint16_t accel_get_timeout (uint8_t index);

void accel_set_active (uint8_t index, uint8_t val);
uint8_t accel_is_active (uint8_t index);

void accel_set_flag_crash (uint8_t index, uint8_t val);
uint8_t accel_get_flag_crash (uint8_t index);

void accel_set_flag_movement (uint8_t index, uint8_t val);
uint8_t accel_get_flag_movement (uint8_t index);

void accel_set_sampling ( uint8_t index, uint16_t val);
uint16_t accel_get_sampling ( uint16_t index );

void accel_set_accel_xyz (uint8_t index, uint16_t accel_x, uint16_t accel_y, uint16_t accel_z);
uint8_t accel_check_accel_xyz ( uint8_t index );






#endif