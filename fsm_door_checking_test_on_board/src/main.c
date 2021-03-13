#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"

#include "fsm.h"
#include "fsm_door_checking.h"
#include "hardware_esp8266.h"

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
    fsm_door_checking_t  left_door_fsm;
    //fsm_door_checking_t  right_door_fsm;
    int alarm_status = 1;
    //uint32_t counter = 0;

    fsm_door_checking_init(&left_door_fsm, GPIO_Pin_4);
    
    //fsm_door_checking_init(&right_door_fsm, 7);

    //testing stuff
    GPIO_AS_OUTPUT(GPIO_Pin_5);
    //GPIO_OUTPUT(D1, 0);

    //GPIO_AS_OUTPUT(GPIO_Pin_7);
    //GPIO_OUTPUT(GPIO_Pin_7, 0);

    //GPIO_DIS_OUTPUT(D2);
    GPIO_AS_INPUT(GPIO_Pin_4);
    //gpio16_input_conf();
    //gpio16_output_conf();

    while (1) {

        /*if (counter == 1000000){
            alarm_status = !alarm_status;
            counter = 0;
        }
        counter++;*/

        //alarm_status = gpio16_input_get();
        alarm_status = GPIO_INPUT_GET(4);
        printf("INPUT VALUE: %d\n", alarm_status);

        fsm_door_checking_turn_on_off_active(&left_door_fsm, alarm_status);
        fsm_fire((fsm_t*)&left_door_fsm);
        //fsm_fire((fsm_t*)&right_door_fsm);

        if ( fsm_door_checking_is_in_alarm (&left_door_fsm) ) {
            GPIO_OUTPUT(GPIO_Pin_5, 1);
            //printf("Alarm_on\n");
        }
        else {
            GPIO_OUTPUT(GPIO_Pin_5, 0);
            //printf("Alarm_off\n");
        }

        //if ( fsm_door_checking_is_in_alarm (&right_door_fsm) ) GPIO_OUTPUT(GPIO_Pin_7, 1);
        //else GPIO_OUTPUT(GPIO_Pin_7, 1);
  
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