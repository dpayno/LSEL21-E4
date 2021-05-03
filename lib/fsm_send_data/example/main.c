#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "gpio.h"

#include "app_config.h"
#include "esp_wifi_station_module.h"
#include "mqtt_hal.h"
#include "fsm_send_data.h"
#include "fsm_door_checking.h"
#include "fsm_hit_detection.h"
#include "fsm_led_alarm.h"

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

void task_example(void* ignore)
{
    // FSM definitions
    fsm_hit_detection_t  fsm_accel[1];
    fsm_led_alarm_t      fsm_leds[1];
    fsm_door_checking_t  fsm_door[1];
    fsm_send_data_t      fsm_send;

    accel_threshold_t hard_threshold;
    accel_threshold_t soft_threshold;
    hard_threshold.x = ACCEL_THRESHOLD_HARD;
    hard_threshold.y = ACCEL_THRESHOLD_HARD;
    hard_threshold.z = ACCEL_THRESHOLD_HARD;
    soft_threshold.x = ACCEL_THRESHOLD_SOFT;
    soft_threshold.y = ACCEL_THRESHOLD_SOFT;
    soft_threshold.z = ACCEL_THRESHOLD_SOFT;

    // FSM initialization
    fsm_hit_detection_init(&fsm_accel[0], hard_threshold, soft_threshold, ACCEL_RATE_MS, ACCEL_SCL_PIN, ACCEL_SDA_PIN);
    fsm_led_alarm_init(&fsm_leds[0], LED_ALARM_PIN, LED_ALARM_RATE_MS);
    fsm_door_checking_init(&fsm_door[0], DOOR_CHECKING_PIN);
    fsm_send_data_init(&fsm_send, MQTT_SEND_RATE_MS, fsm_leds, 1, fsm_door, 1, fsm_accel, 1);

    // WiFi connection
    esp_wifi_station_t station;
    esp_wsm_init_module_station(&station, AP_SSID, AP_PASSWORD);
    esp_wsm_connect_to_ap();
    printf("Connecting...");
    do {
        printf(".");
        vTaskDelay(500/portTICK_RATE_MS);
    } while (!esp_wsm_station_connected());
    printf("\nConnected to AP.\n");

    // MQTT connection & initialization
    mqtt_hal_init(&(fsm_send.client), MQTT_PORT, MQTT_BROKER, MQTT_CLIENT, &fsm_send);
    mqtt_hal_suscribe(&(fsm_send.client), MQTT_SUSCRIBE_TOPIC);
    printf("Configuration finished!\n");
    fsm_send_data_set_active(&fsm_send, 1);
    fsm_fire((fsm_t*)&fsm_send);

    // FSM loop
    while (1) {
        fsm_fire((fsm_t*)&fsm_accel[0]);
        fsm_fire((fsm_t*)&fsm_leds[0]);
        fsm_fire((fsm_t*)&fsm_door[0]);
        fsm_fire((fsm_t*)&fsm_send);
        mqtt_hal_fire(&(fsm_send.client));
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
    xTaskCreate(&task_example, "startup", 2048, NULL, 1, NULL);
}
