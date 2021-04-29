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
    if (data->message->payload == "ACTIVATE_SYSTEM")
    {
        global_active = 1;
    }
    else if (data->message->payload == "DISABLE_SYSTEM")
    {
        global_active = 0;
    }
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
    

/********************************************************* ESTO NO SE HA PROBADO***************************************/
	Network network;
	unsigned char sendbuf[80], readbuf[80];
	int rc = 0,
	count = 0;
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

	//pvParameters = 0;
	NetworkInit(&network);
	MQTTClientInit(&(send.client), &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

	char* address = "192.168.0.36"; 
	if ((rc = ConnectNetwork(&network, address, 8883)) != 0)
		printf("Return code from network connect is %d\n", rc);

#if defined(MQTT_TASK)
	if ((rc = MQTTStartTask(&(send.client)) != pdPASS))
		printf("Return code from start tasks is %d\n", rc);
#endif

	connectData.MQTTVersion = 3;
	connectData.clientID.cstring = "FreeRTOS_sample";

	if ((rc = MQTTConnect(&(send.client), &connectData)) != 0)
		printf("Return code from MQTT connect is %d\n", rc);
	else
		printf("MQTT Connected\n");

	if ((rc = MQTTSubscribe(&(send.client), "alarm_status/#", 2, messageArrived)) != 0)
		printf("Return code from MQTT subscribe is %d\n", rc);
/********************************************************* ESTO NO SE HA PROBADO***************************************/

    /*fsm_send_data_set_active(&send, 1);*/
    while (1) {
        if (global_active)
        {
          fsm_send_data_set_active(&send, 1);
        }
        else if (!global_active)
        {
          fsm_send_data_set_active(&send, 0);
        }
        
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
