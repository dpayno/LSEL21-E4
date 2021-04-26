#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c_master.h"
#include "gpio.h"
#include "fsm.h"
#include "fsm_hit_detection.h"
#include "accelerometer.h"

#define HIT_THRESHOLD_HARD 0xFFFF >> 2
#define HIT_THRESHOLD_SOFT 0xFFFF >> 3
#define HIT_RATE 100

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

void task_hit_detection(void* ignore)
{
    int active = 1;
    fsm_hit_detection_t  accel_1;
    accel_threshold_t hard_threshold;
    accel_threshold_t soft_threshold;

    hard_threshold.x = HIT_THRESHOLD_HARD;
    hard_threshold.y = HIT_THRESHOLD_HARD;
    hard_threshold.z = HIT_THRESHOLD_HARD;

    soft_threshold.x = HIT_THRESHOLD_SOFT;
    soft_threshold.y = HIT_THRESHOLD_SOFT;
    soft_threshold.z = HIT_THRESHOLD_SOFT;

    fsm_hit_detection_init(&accel_1, hard_threshold, soft_threshold, HIT_RATE, I2C_MASTER_SCL_GPIO, I2C_MASTER_SDA_GPIO);
    fsm_hit_detection_set_active(&accel_1, active);

    while (1) {
        fsm_fire((fsm_t*)&accel_1);
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
    xTaskCreate(&task_hit_detection, "startup", 2048, NULL, 1, NULL);
}
