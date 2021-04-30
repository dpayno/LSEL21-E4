#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "gpio.h"

#include "MQTTClient.h"
#include "fsm_send_data.h"
#include "fsm_door_checking.h"
#include "fsm_hit_detection.h"
#include "fsm_led_alarm.h"

#define AP_SSID     "vodafone8C84"
#define AP_PASSWORD "XXXX"

int global_active = 0;

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



void messageArrived(MessageData* data)
{
    int len = (int)data->message->payloadlen;
    char rx_msg[len];
    memcpy(rx_msg, data->message->payload, len);
    printf("Received msg: %s\n", rx_msg);
    printf("Size of data: %d\n",(int)data->message->payloadlen );

    global_active = !global_active;
}


void task_hit_detection(void* ignore)
{
    int active = 1;
    fsm_send_data_t fsm_send;
    fsm_hit_detection_t  fsm_accel_1[1];
    fsm_led_alarm_t fsm_leds[2];
    fsm_door_checking_t fsm_door[1];

    fsm_led_alarm_init(&fsm_leds[0], D4, 100);
    fsm_led_alarm_init(&fsm_leds[1], D4, 500);
    fsm_send_data_init(&fsm_send, 100, fsm_leds, 2, fsm_door, 1, fsm_accel_1, 1 );

    struct station_config sta_config;
    struct ip_info ip_config;
    bzero(&sta_config, sizeof(struct station_config));

    sprintf(sta_config.ssid, AP_SSID);
    sprintf(sta_config.password, AP_PASSWORD);

    wifi_station_set_config(&sta_config);
    os_printf("%s\n", __func__);
    wifi_get_ip_info(STATION_IF, &ip_config);

    while(ip_config.ip.addr == 0) {
        vTaskDelay(1000 / portTICK_RATE_MS);
        wifi_get_ip_info(STATION_IF, &ip_config);
        printf("Connecting...\n");
    }
    printf("Connected\n");

	Network network;
	unsigned char sendbuf[80], readbuf[80];
	int rc = 0,
	count = 0;
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

	NetworkInit(&network);
	MQTTClientInit(&(fsm_send.client), &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

	char* address = "192.168.0.110"; 
	if ((rc = ConnectNetwork(&network, address, 8883)) != 0)
		printf("Return code from network connect is %d\n", rc);

#if defined(MQTT_TASK)
	if ((rc = MQTTStartTask(&(fsm_send.client)) != pdPASS))
		printf("Return code from start tasks is %d\n", rc);
#endif

	connectData.MQTTVersion = 3;
	connectData.clientID.cstring = "ESP8266_client_example";

	if ((rc = MQTTConnect(&(fsm_send.client), &connectData)) != 0)
		printf("Return code from MQTT connect is %d\n", rc);
	else
		printf("MQTT Connected\n");

	if ((rc = MQTTSubscribe(&(fsm_send.client), "alarm_status/#", 2, messageArrived)) != 0)
		printf("Return code from MQTT subscribe is %d\n", rc);

    while (1) {
        if (global_active) {
          fsm_send_data_set_active(&fsm_send, 1);
        } else if (!global_active) {
          fsm_send_data_set_active(&fsm_send, 0);
        }
        
        fsm_fire((fsm_t*)&fsm_send);
        fsm_fire((fsm_t*)&fsm_leds[0]);
        fsm_fire((fsm_t*)&fsm_leds[1]);

#if !defined(MQTT_TASK)
		if ((rc = MQTTYield(&fsm_send.client, 1000)) != 0)
			printf("Return code from yield is %d\n", rc);
#endif
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
