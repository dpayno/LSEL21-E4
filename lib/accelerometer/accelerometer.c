#include "accelerometer.h"

typedef union {
    struct 
    {
        unsigned active      :1;
        unsigned pin_SLC     :5;
        unsigned pin_SDA     :5;
        unsigned flag_crash  :1;
    };
    uint8_t data;
}flags_accel_t;

typedef struct 
{
  flags_accel_t flags_accel;
  uint16_t timeout;
  uint16_t sampling_ms;
  uint16_t threshold_x;
  uint16_t threshold_y;
  uint16_t threshold_z;
  uint16_t accel_x;
  uint16_t accel_y;
  uint16_t accel_z;
}flags_and_data_accel_t;

static flags_and_data_accel_t flags_and_data_accel[NAccel];
static uint8_t m_u8_index = 0;

uint8_t get_new_index ()
{
  flags_and_data_accel[m_u8_index].flags_accel.data = 0x00;
  flags_and_data_accel[m_u8_index].timeout = 0;
  return m_u8_index++;
}

void accel_set_thresholds ( uint8_t index, uint16_t threshold_x, uint16_t threshold_y, uint16_t threshold_z )
{   
    flags_and_data_accel[index].threshold_x = threshold_x;
    flags_and_data_accel[index].threshold_y = threshold_y;
    flags_and_data_accel[index].threshold_z = threshold_z;
}

void accel_set_pins ( uint8_t index, uint8_t pin_SDA, uint8_t pin_SLC )
{
    /*IMPLEMENTAR CONFIGURACIÓN PARA I2C EN CASO DE LLEVAR*/
    flags_and_data_accel[index].flags_accel.pin_SDA = pin_SDA;
    flags_and_data_accel[index].flags_accel.pin_SLC = pin_SLC;
}

void accel_set_timeout ( uint8_t index, uint16_t val)
{
    flags_and_data_accel[index].timeout = val;
}

uint16_t accel_get_timeout (uint8_t index)
{
    return flags_and_data_accel[index].timeout;
}


void accel_set_active (uint8_t index, uint8_t val)
{
    flags_and_data_accel[index].flags_accel.active = val;
}

uint8_t accel_is_active (uint8_t index)
{
    return flags_and_data_accel[index].flags_accel.active;
}


void accel_set_flag_crash (uint8_t index, uint8_t val)
{
    flags_and_data_accel[index].flags_accel.flag_crash = val;
}

uint8_t accel_get_flag_crash (uint8_t index)
{
    return flags_and_data_accel[index].flags_accel.flag_crash;
}


void accel_set_sampling ( uint8_t index, uint16_t val)
{
    flags_and_data_accel[index].sampling_ms = val;
}

uint16_t accel_get_sampling ( uint16_t index )
{
    return flags_and_data_accel[index].sampling_ms;
}


void accel_get_accel_xyz (uint8_t index, uint16_t accel_x, uint16_t accel_y, uint16_t accel_z)
{
    flags_and_data_accel[index].accel_x = accel_x;
    flags_and_data_accel[index].accel_y = accel_y;
    flags_and_data_accel[index].accel_z = accel_z;
}


uint8_t accel_check_accel_xyz ( uint8_t index )
{
    return ((flags_and_data_accel[index].accel_x >= flags_and_data_accel[index].threshold_x) || (flags_and_data_accel[index].accel_y >= flags_and_data_accel[index].threshold_y) || (flags_and_data_accel[index].accel_z >= flags_and_data_accel[index].threshold_z));
}
















