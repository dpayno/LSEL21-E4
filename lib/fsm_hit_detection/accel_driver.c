#include "accel_driver.h"
#include "mpu6050_driver.h"

int32_t accel_driver_init(uint8_t pin_SDA, uint8_t pin_SLC)
{
    return mpu6050_i2c_init();
}

int32_t accel_driver_get_xyz(uint16_t* x, uint16_t* y, uint16_t* z)
{
    int8_t status = 0;
    uint8_t sensor_data[6];

    // Read MPU6050 accel data
    status = mpu6050_i2c_read(sensor_data, 6);

    // Check if MPU is not initialized
    int sum = 0;
    for (int i = 0; i < 6; i++) {
        sum += sensor_data[i];
    }

    // Init MPU if it wasn't and re-read
    if (!sum) {
        mpu6050_i2c_init();
        status = mpu6050_i2c_read(sensor_data, 6);
    }

    // Fill data
    *x = sensor_data[0] << 8 | sensor_data[1];
    *y = sensor_data[2] << 8 | sensor_data[3];
    *z = sensor_data[4] << 8 | sensor_data[5];

    return status;
}