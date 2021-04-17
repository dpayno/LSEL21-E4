#ifndef ACCELEROMETER__H
#define ACCELEROMETER__H

#include <stdio.h>
#include "stdlib.h"
#include "stdint.h"
#include "hardware_esp8266.h"

#define NAccel 1


uint8_t get_new_index ();
void accel_set_thresholds ( uint8_t index, uint16_t threshold_x, uint16_t threshold_y, uint16_t threshold_z );
void accel_set_pins ( uint8_t index, uint8_t pin_SDA, uint8_t pin_SLC );

void accel_set_timeout ( uint8_t index, uint16_t new_timeout);
uint16_t accel_get_timeout (uint8_t index);

void accel_set_active (uint8_t index, uint8_t val);
uint8_t accel_is_active (uint8_t index);

void accel_set_flag_crash (uint8_t index, uint8_t val);
uint8_t accel_get_flag_crash (uint8_t index);

void accel_set_sampling ( uint8_t index, uint16_t val);
uint16_t accel_get_sampling ( uint16_t index );

void accel_get_accel_xyz (uint8_t index, uint16_t accel_x, uint16_t accel_y, uint16_t accel_z);
uint8_t accel_check_accel_xyz ( uint8_t index );






#endif