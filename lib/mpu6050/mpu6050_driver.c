#include "mpu6050_driver.h"
#include "c_types.h"

#include "i2c_master.h"
#include <stdio.h>

void mpu6050_i2c_init()
{
    i2c_master_gpio_init();
    i2c_master_init();
}

bool mpu6050_begin_transmision()
{
    uint8_t i2c_addr_write = (MPU6050_SENSOR_ADDR << 1);
    
    i2c_master_start();
    i2c_master_writeByte(i2c_addr_write);
    
    return i2c_master_checkAck();
}

void mpu6050_end_transmision()
{
    i2c_master_stop();
}

bool mpu6050_write(uint8_t i2c_data)
{
    i2c_master_writeByte(i2c_data);
    
    return i2c_master_checkAck();
}

bool mpu6050_request_from(uint8_t i2c_addr)
{
    uint8_t i2c_addr_read = (i2c_addr << 1) + 1;
    
    i2c_master_start();
    i2c_master_writeByte(i2c_addr_read);
    
    return i2c_master_checkAck();
}

bool mpu6050_read(uint8_t num_bytes, int8_t* data)
{
    if (num_bytes < 1 || data == NULL) return false;

    int i;
    for(i = 0; i < num_bytes - 1; i++)
    {
        data[i] = i2c_master_readByte();
        i2c_master_send_ack();
    }
    // nack the final packet so that the slave releases SDA
    data[num_bytes - 1] = i2c_master_readByte();
    i2c_master_send_nack();

    return true;
}

void mpu_get_raw_data()
{
    int8_t raw_packets[6] = {0, 0, 0, 0, 0, 0};

    if (mpu6050_begin_transmision()) {
        printf("There is communication!\n");
    } else {
        printf("There is NO communication :(\n");
    }
    mpu6050_end_transmision();

    mpu6050_request_from(ACCEL_XOUT_H);
    mpu6050_read(6, raw_packets);
    mpu6050_end_transmision();

    for (int i = 0; i < 6; i++) {
        printf("Byte %d value: %d\n", i, raw_packets[i]);
    }
}

