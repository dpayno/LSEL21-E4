#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "mpu6050_driver.h"

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void i2c_test_task()
{
    uint8_t sensor_data[14];
    int sample;
    int8_t status;
    uint16 accel_x;
    uint16 accel_y;
    uint16 accel_z;
    uint16 temperature;
    uint16 gyro_x;
    uint16 gyro_y;
    uint16 gyro_z;

    printf("Init MPU6050... \n");
    status = mpu6050_i2c_init();
    printf("Init status : %d \n", status);


    while (1) {

        status = mpu6050_i2c_read(sensor_data, 14);
        sample++;

        accel_x = sensor_data[0] << 8 | sensor_data[1];
        accel_y = sensor_data[2] << 8 | sensor_data[3];
        accel_z = sensor_data[4] << 8 | sensor_data[5];
        temperature = sensor_data[6] << 8 | sensor_data[7];
        gyro_x = sensor_data[8] << 8 | sensor_data[9];
        gyro_y = sensor_data[10] << 8 | sensor_data[11];
        gyro_z = sensor_data[12] << 8 | sensor_data[13];

        printf("\tSample : %d\n", sample);
        printf("\tStatus : %d \n", status);
        printf("\taccel_x : %d,\t accel_y : %d,\t accel_z : %d\n", accel_x, accel_y, accel_z);
        printf("\tgyro_x : %d,\t gyro_y : %d,\t gyro_z : %d\n", gyro_x, gyro_y, gyro_z);
        printf("\tTemperature : %d\n", temperature);
        printf("\t----------------------------------\n");

        vTaskDelay(500 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}


/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    xTaskCreate(&i2c_test_task, "startup", 2048, NULL, 1, NULL);
}
