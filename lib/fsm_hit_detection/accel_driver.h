#ifndef ACCEL_DRIVER_H
#define ACCEL_DRIVER_H

#include <stdint.h>

int32_t accel_driver_init(uint8_t pin_SDA, uint8_t pin_SLC);

int32_t accel_driver_get_xyz(uint16_t* x, uint16_t* y, uint16_t* z);


#endif // ACCEL_DRIVER_H