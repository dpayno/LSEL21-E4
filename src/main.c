#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "esp_wifi_station_module.h"
#include "mpu6050_driver.h"

#define ap_name        "monde38"
#define ap_password    "monde.38"

#define mpu6050 0x68

uint8 Acknowledge;
uint8 pData[14];
uint8 count;

uint16 Acx;
uint8 Accelerometer_X1;
uint8 Accelerometer_X2;

uint16 Acy;
uint8 Accelerometer_Y1;
uint8 Accelerometer_Y2;

uint16 Acz;
uint8 Accelerometer_Z1;
uint8 Accelerometer_Z2;

uint16 Tem;
uint8 Temperature_1;
uint8 Temperature_2;

uint16 Gyx;
uint8 Gyroscope_X1;
uint8 Gyroscope_X2;

uint16 Gyy;
uint8 Gyroscope_Y1;
uint8 Gyroscope_Y2;
	
uint16 Gyz;
uint8 Gyroscope_Z1;
uint8 Gyroscope_Z2;

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

    i2c_master_gpio_init();
    i2c_master_start();

    //7 bit Address + LSB=0 for write command
    i2c_master_writeByte(mpu6050 << 1 | 0);
    Acknowledge = i2c_master_getAck();
    printf("ACK: %d\n", Acknowledge);

    //Power Mode
    i2c_master_writeByte(0x6B);
    Acknowledge = i2c_master_getAck();

    //On MPU6050
    i2c_master_writeByte(0x00);
    Acknowledge = i2c_master_getAck();

    i2c_master_stop();
    i2c_master_start();

    while (1) {
        i2c_master_writeByte(0x68 << 1 | 0);
        Acknowledge = i2c_master_getAck();
        i2c_master_writeByte(0x3B);
        Acknowledge = i2c_master_getAck();
        i2c_master_start();
        i2c_master_writeByte(0x68 << 1 | 1);
        Acknowledge = i2c_master_getAck();

        printf("start MPU6050\n");
        Accelerometer_X1 = i2c_master_readByte();
        i2c_master_setAck(0);
        Accelerometer_X2 = i2c_master_readByte();
        i2c_master_setAck(0);
        Accelerometer_Y1 = i2c_master_readByte();
        i2c_master_setAck(0);
        Accelerometer_Y2 = i2c_master_readByte();
        i2c_master_setAck(0);
        Accelerometer_Z1 = i2c_master_readByte();
        i2c_master_setAck(0);
        Accelerometer_Z2 = i2c_master_readByte();
        i2c_master_setAck(0);

        Temperature_1 = i2c_master_readByte();
        i2c_master_setAck(0);
        Temperature_2 = i2c_master_readByte();
        i2c_master_setAck(0);

        Gyroscope_X1 = i2c_master_readByte();
        i2c_master_setAck(0);
        Gyroscope_X2 = i2c_master_readByte();
        i2c_master_setAck(0);
        Gyroscope_Y1 = i2c_master_readByte();
        i2c_master_setAck(0);
        Gyroscope_Y2 = i2c_master_readByte();
        i2c_master_setAck(0);
        Gyroscope_Z1 = i2c_master_readByte();
        i2c_master_setAck(0);
        Gyroscope_Z2 = i2c_master_readByte();
        i2c_master_setAck(1);
        Acx = Accelerometer_X1 << 8 | Accelerometer_X2;
        Acy = Accelerometer_Y1 << 8 | Accelerometer_Y2;
        Acz = Accelerometer_Z1 << 8 | Accelerometer_Z2;

        Tem = Temperature_1 << 8 | Temperature_2;
        Gyx = Gyroscope_X1 << 8 | Gyroscope_X2;
        Gyy = Gyroscope_Y1 << 8 | Gyroscope_Y2;
        Gyz = Gyroscope_Z1 << 8 | Gyroscope_Z2;

        printf("stop MPU6050\n");
        printf("Acx : %d\n", Acx);
        printf("Acy : %d\n", Acy);
        printf("Acz : %d\n", Acz);
        printf("Tem : %d\n", Tem);
        printf("Gyx : %d\n", Gyx);
        printf("Gyy : %d\n", Gyy);
        printf("Gyz : %d\n", Gyz);
        vTaskDelay(200);  //Delay for 200milli seconds
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
