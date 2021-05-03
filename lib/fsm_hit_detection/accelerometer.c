#include "accelerometer.h"
#include <stdlib.h>

typedef union {
    struct 
    {
        unsigned active        :1;
        unsigned pin_SCL       :5;
        unsigned pin_SDA       :5;
        unsigned flag_crash    :1;
        unsigned flag_movement :1;
    };
    uint8_t data;
} flags_accel_t;

typedef struct 
{
  flags_accel_t flags_accel;
  uint16_t timeout;
  uint16_t sampling_ms;
  accel_threshold_t threshold_hard;
  accel_threshold_t threshold_soft;
  uint16_t accel_x_now;
  uint16_t accel_y_now;
  uint16_t accel_z_now;
  uint16_t accel_x_last;
  uint16_t accel_y_last;
  uint16_t accel_z_last;
} flags_and_data_accel_t;

static flags_and_data_accel_t flags_and_data_accel[N_ACCEL];
static uint8_t m_u8_index = 0;

uint8_t accel_get_new_index ()
{
  flags_and_data_accel[m_u8_index].flags_accel.data = 0x00;
  flags_and_data_accel[m_u8_index].timeout = 0;
  return m_u8_index++;
}

void accel_set_thresholds ( uint8_t index, accel_threshold_t hard, accel_threshold_t soft)
{   
    flags_and_data_accel[index].threshold_hard.x = hard.x;
    flags_and_data_accel[index].threshold_hard.y = hard.y;
    flags_and_data_accel[index].threshold_hard.z = hard.z;

    flags_and_data_accel[index].threshold_soft.x = soft.x;
    flags_and_data_accel[index].threshold_soft.y = soft.y;
    flags_and_data_accel[index].threshold_soft.z = soft.z;
}

void accel_set_pins ( uint8_t index, uint8_t pin_SDA, uint8_t pin_SCL )
{
    /*IMPLEMENTAR CONFIGURACIÓN PARA I2C EN CASO DE LLEVAR*/
    flags_and_data_accel[index].flags_accel.pin_SDA = pin_SDA;
    flags_and_data_accel[index].flags_accel.pin_SCL = pin_SCL;
}

int8_t accel_get_pins ( uint8_t index, uint8_t* pin_SDA, uint8_t* pin_SCL )
{
    *pin_SDA = flags_and_data_accel[index].flags_accel.pin_SDA;
    *pin_SCL = flags_and_data_accel[index].flags_accel.pin_SCL;
    return 0;
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

void accel_set_flag_movement (uint8_t index, uint8_t val)
{
    flags_and_data_accel[index].flags_accel.flag_movement = val;
}

uint8_t accel_get_flag_movement (uint8_t index)
{
    return flags_and_data_accel[index].flags_accel.flag_movement;
}


void accel_set_sampling ( uint8_t index, uint16_t val)
{
    flags_and_data_accel[index].sampling_ms = val;
}

uint16_t accel_get_sampling ( uint16_t index )
{
    return flags_and_data_accel[index].sampling_ms;
}


void accel_set_accel_xyz (uint8_t index, uint16_t accel_x, uint16_t accel_y, uint16_t accel_z)
{
    flags_and_data_accel[index].accel_x_last = flags_and_data_accel[index].accel_x_now;
    flags_and_data_accel[index].accel_y_last = flags_and_data_accel[index].accel_y_now;
    flags_and_data_accel[index].accel_z_last = flags_and_data_accel[index].accel_z_now;
    flags_and_data_accel[index].accel_x_now = accel_x;
    flags_and_data_accel[index].accel_y_now = accel_y;
    flags_and_data_accel[index].accel_z_now = accel_z;
}

void accel_get_accel_xyz (uint8_t index, uint16_t* accel_x, uint16_t* accel_y, uint16_t* accel_z)
{
    *accel_x = flags_and_data_accel[index].accel_x_now;
    *accel_y = flags_and_data_accel[index].accel_y_now;
    *accel_z = flags_and_data_accel[index].accel_z_now;
}


uint8_t accel_check_accel_hard_xyz ( uint8_t index )
{
    return  ((abs((int16_t)(flags_and_data_accel[index].accel_x_now - flags_and_data_accel[index].accel_x_last)) >= flags_and_data_accel[index].threshold_hard.x) ||
             (abs((int16_t)(flags_and_data_accel[index].accel_y_now - flags_and_data_accel[index].accel_y_last)) >= flags_and_data_accel[index].threshold_hard.y) ||
             (abs((int16_t)(flags_and_data_accel[index].accel_z_now - flags_and_data_accel[index].accel_z_last)) >= flags_and_data_accel[index].threshold_hard.z));
}

uint8_t accel_check_accel_soft_xyz ( uint8_t index )
{
    return  ((abs((int16_t)(flags_and_data_accel[index].accel_x_now - flags_and_data_accel[index].accel_x_last)) >= flags_and_data_accel[index].threshold_soft.x) ||
             (abs((int16_t)(flags_and_data_accel[index].accel_y_now - flags_and_data_accel[index].accel_y_last)) >= flags_and_data_accel[index].threshold_soft.y) ||
             (abs((int16_t)(flags_and_data_accel[index].accel_z_now - flags_and_data_accel[index].accel_z_last)) >= flags_and_data_accel[index].threshold_soft.z));
}
