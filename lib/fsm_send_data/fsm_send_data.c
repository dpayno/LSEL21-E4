#include "fsm_send_data.h"
#include "esp_wifi_station_module.h"


static int is_active_system(fsm_t* this);
static int is_timeout(fsm_t* this);
static int is_not_active_system(fsm_t* this);
static void activate_system(fsm_t* this);
static void send_data(fsm_t* this);
static void disable_system(fsm_t* this);





/* Tabla de transiciones ******************************************/
static fsm_trans_t tabla_trans[] = {
        { IDLE,    is_active_system,     SENDING, activate_system },
        { SENDING, is_timeout,           SENDING, send_data       },
        { SENDING, is_not_active_system, IDLE,    disable_system  },
        { -1,      NULL,                 -1,      NULL            },
};


void fsm_send_data_init(fsm_send_data_t* this, uint16_t sampling_ms, fsm_led_alarm_t fsm_led[], uint8_t n_leds, fsm_door_checking_t fsm_door[], uint8_t n_doors, fsm_hit_detection_t fsm_hit[], uint8_t n_accel)
{
    fsm_init ((fsm_t *) this, tabla_trans);
    this->sampling_ms = sampling_ms;
    this->timeout = 0;
    this->n_leds = n_leds;
    this->n_accel = n_accel;
    this->n_doors = n_doors;

        for (int i = 0; i < this->n_leds; i++)
        {
            this->fsm_led[i] = fsm_led[i];
        }

        for (int i = 0; i < this->n_accel; i++)
        {
            this->fsm_hit[i] = fsm_hit[i]; 
        }  

         for (int i = 0; i < this->n_doors; i++)
        {
            this->fsm_door[i] = fsm_door[i]; 
        }
    
    esp_wifi_station_module station;
    init_module_station(&station);
    connect_to_ap();
    printf("Connecting...");
    do
    {
    printf(".");
    vTaskDelay(500/portTICK_RATE_MS);
    } while (station_connected() != 1);
    printf("\nCONECTED!\n");

}


void fsm_send_data_set_active (fsm_send_data_t* this, uint8_t val)
{
    this->active = val;
}


static int is_active_system(fsm_t* this)
{
    fsm_send_data_t *f = (fsm_send_data_t*) this;
    return f->active;
}


static int is_timeout(fsm_t* this)
{
    fsm_send_data_t *f = (fsm_send_data_t*) this;
    return ((get_tick_count() >= f->timeout) && f->active);
}


static int is_not_active_system(fsm_t* this)
{
    fsm_send_data_t *f = (fsm_send_data_t*) this;
    return !f->active;
}


static void activate_system(fsm_t* this)
{
    fsm_send_data_t *f = (fsm_send_data_t*) this;
    
        for (int i = 0; i < f->n_leds; i++)
        {
            fsm_led_alarm_turn_on_off_active(&(f->fsm_led[i]), 1); 
        }

        for (int i = 0; i < f->n_accel; i++)
        {
            fsm_hit_detection_set_active(&(f->fsm_hit[i]), 1); 
        }  

        for (int i = 0; i < f->n_doors; i++)
        {
            fsm_door_checking_turn_on_off_active (&(f->fsm_door[i]), 1); 
        }

    f->timeout = get_tick_count() + f->sampling_ms; /*INIT TIMER*/
}


static void send_data(fsm_t* this)
{
    fsm_send_data_t *f = (fsm_send_data_t*) this;

     /******************************* ESTO NO SE HA PROBADO******************/
    int rc = 0;
    MQTTMessage message;
	char payload[] = "HOLA AMIGO";
	message.qos = 1;
	message.retained = 0;
	message.payload = payload;
	message.payloadlen = strlen(payload);

		if ((rc = MQTTPublish(&(f->client), "sensor_data", &message)) != 0)
			printf("Return code from MQTT publish is %d\n", rc);

		if ((rc = MQTTYield(&(f->client), 1000)) != 0)
			printf("Return code from yield is %d\n", rc);
    /*********************************ESTO NO SE HA PROBADO******************/

    f->timeout = get_tick_count() + f->sampling_ms;
}


static void disable_system(fsm_t* this)
{  
    fsm_send_data_t *f = (fsm_send_data_t*) this;
    
        for (int i = 0; i < f->n_leds; i++)
        {
            fsm_led_alarm_turn_on_off_active(&(f->fsm_led[i]), 0); 
        }

         for (int i = 0; i < f->n_accel; i++)
        {
            fsm_hit_detection_set_active(&(f->fsm_hit[i]), 0); 
        }  

        for (int i = 0; i < f->n_doors; i++)
        {
            fsm_door_checking_turn_on_off_active (&(f->fsm_door[i]), 0); 
        }
}






