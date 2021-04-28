#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "gpio.h"
#include "esp_wifi_station_module.h"
#include "MQTTClient.h"
#include "fsm_send_data.h"
#include "fsm_door_checking.h"
#include "fsm_hit_detection.h"
#include "fsm_led_alarm.h"



#define AP_SSID     "vodafone8C84"
#define AP_PASSWORD "XXXX"


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
    fsm_hit_detection_t  accel_1[1];
    fsm_led_alarm_t leds[2];
    fsm_led_alarm_init(&leds[0], D4, 100);
    fsm_led_alarm_init(&leds[1], D4, 500);
    fsm_door_checking_t door[1];
    fsm_send_data_t send;
    
    fsm_send_data_init(&send, 100, leds, 2, door, 1, accel_1, 1 );
    fsm_send_data_set_active(&send, 1);
    while (1) {
        fsm_fire((fsm_t*)&send);
        fsm_fire((fsm_t*)&leds[0]);
        fsm_fire((fsm_t*)&leds[1]);
        
        
        

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
