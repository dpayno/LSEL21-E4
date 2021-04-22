#include "mpu6050_driver.h"
#include "c_types.h"

#include "i2c_master.h"
#include <stdio.h>

#include "esp_common.h"
#include "freertos/task.h"

int32_t mpu6050_i2c_init()
{
    uint8_t mpu_init_data[] = {0x00};
    i2c_master_gpio_init();
    return mpu6050_i2c_write(PWR_MGMT_1, mpu_init_data, 1);
}

int32_t mpu6050_i2c_write(uint8_t reg, uint8_t* data, uint8_t length)
{
    // S
    i2c_master_start();

    // AD + W
    i2c_master_writeByte(MPU6050_SENSOR_ADDR << 1);
    if (!i2c_master_checkAck()) { // ACK
        i2c_master_stop();
        return -1;
    }

    // RA
    i2c_master_writeByte(reg);
    if (!i2c_master_checkAck()) { // ACK
        i2c_master_stop();
        return -2;
    }

    // DATA
    for (int i = 0; i < length; i++) {
        i2c_master_writeByte(data[i]);
        if (!i2c_master_checkAck()) { // ACK
            i2c_master_stop();
            return -3;
        }
    }

    // P
    i2c_master_stop();

    return 0;
}

int32_t mpu6050_i2c_read(uint8_t *data, uint16_t length)
{
    // S
    i2c_master_start();

    // AD + W
    i2c_master_writeByte(MPU6050_SENSOR_ADDR << 1);
    if (!i2c_master_checkAck()) { // ACK
        i2c_master_stop();
        return -1;
    }

    // RA
    i2c_master_writeByte(ACCEL_XOUT_H);
    if (!i2c_master_checkAck()) { // ACK
        i2c_master_stop();
        return -2;
    }

    // S
    i2c_master_start();

    // AD + R
    i2c_master_writeByte(MPU6050_SENSOR_ADDR << 1 | 1);

    if (!i2c_master_checkAck()) { // ACK
        i2c_master_stop();
        return -3;
    }
    for (int i = 0; i < length - 1; i++) {
        // DATA
        data[i] = i2c_master_readByte();
        // ACK
        i2c_master_send_ack();
    }

    // DATA
    data[length] = i2c_master_readByte();
    // NACK
    i2c_master_send_nack();

    // P
    i2c_master_stop();

    return 0;
}