#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"

#include "fsm.h"
#include "fsm_hit_detection.h"
#include "hardware_esp8266.h"
#include "accelerometer.h"

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

void task_blink(void* ignore)
{
    int active = 1;
    int threshold_x = 100;
    int threshold_y = 100;
    int threshold_z = 100;
    int sampling_ms = 1000;
    fsm_hit_detection_t  accel_1;
    fsm_hit_detection_init(&accel_1, threshold_x, threshold_y, threshold_z, sampling_ms, D1, D2);
    activate_system(&accel_1, active);

    while (1) 
    {
        fsm_fire((fsm_t*)&accel_1);
        if ( fsm_hit_detection_alarm (&accel_1)) 
        { 
            printf("THRESHOLDS SOBREPASADO\n\n");
        }
        else 
        {
            printf("THRESHOLDS NO SOBREPASADO\n\n");
        }
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
    xTaskCreate(&task_blink, "startup", 2048, NULL, 1, NULL);
}