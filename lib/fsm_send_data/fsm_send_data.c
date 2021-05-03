#include "fsm_send_data.h"
#include "esp_wifi_station_module.h"
#include "cJSON.h"

#define MQTT_PUBLISH_TOPIC "sensor_data"

/* Definición funciones privadas *********************************************/
// Funciones de guarda
static int is_active_system(fsm_t* this);
static int is_timeout(fsm_t* this);
static int is_not_active_system(fsm_t* this);
// Funciones de salida
static void activate_system(fsm_t* this);
static void send_data(fsm_t* this);
static void disable_system(fsm_t* this);

/* Tabla de transiciones *****************************************************/
static fsm_trans_t tabla_trans[] = {
    { IDLE,    is_active_system,     SENDING, activate_system },
    { SENDING, is_timeout,           SENDING, send_data       },
    { SENDING, is_not_active_system, IDLE,    disable_system  },
    { -1,      NULL,                 -1,      NULL            },
};

/* Implementación funciones privadas *****************************************/
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
    
    for (int i = 0; i < f->n_leds; i++) {
        fsm_led_alarm_turn_on_off_active(&(f->fsm_led[i]), 1); 
    }
    for (int i = 0; i < f->n_accel; i++) {
        fsm_hit_detection_set_active(&(f->fsm_hit[i]), 1); 
    }  
    for (int i = 0; i < f->n_doors; i++) {
        fsm_door_checking_turn_on_off_active (&(f->fsm_door[i]), 1); 
    }

    f->timeout = get_tick_count() + f->sampling_ms; /*INIT TIMER*/
}

static void send_data(fsm_t* this)
{
    fsm_send_data_t *f = (fsm_send_data_t*) this;

    cJSON* root = cJSON_CreateObject();

    //MAC
    uint8_t mac_address[6];
    wifi_get_macaddr(0x00, mac_address);
    const int mac[6] = {mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]};
    cJSON_AddItemToObject(root, "macaddress", cJSON_CreateIntArray(mac, 6));

    // DOOR
    cJSON* door = cJSON_CreateObject();
    cJSON* door_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "door", door_array);
    for (int i = 0; i < f->n_doors; i++) {
        cJSON* door_item = cJSON_CreateObject();
        cJSON_AddItemToArray(door_array, door_item);
        cJSON_AddNumberToObject(door_item, "id", f->fsm_door[i].u8_index);
        cJSON_AddStringToObject(door_item, "status", is_active_system(this) ? "on" : "off");
        cJSON_AddStringToObject(door_item, "door_state", fsm_door_checking_is_in_alarm (&(f->fsm_door[i])) ? "open" : "close");
    }

    // HIT
    cJSON* hit = cJSON_CreateObject();
    cJSON* hit_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "hit", hit_array);
    for (int i = 0; i < f->n_accel; i++) {
        cJSON* hit_item = cJSON_CreateObject();
        cJSON_AddItemToArray(hit_array, hit_item);
        cJSON_AddNumberToObject(hit_item, "id", f->fsm_hit[i].u8_index);
        cJSON_AddStringToObject(hit_item, "status", is_active_system(this) ? "on" : "off");
        cJSON_AddStringToObject(hit_item, "hit", accel_get_flag_crash (i) ? "yes" : "no");
    }

    //MOVEMENT
    cJSON* movement = cJSON_CreateObject();
    cJSON* movement_array = cJSON_CreateArray();
    int16_t x, y, z;
    cJSON_AddItemToObject(root, "movement", movement_array);
    for (int i = 0; i < f->n_accel; i++) {
        cJSON* movement_item = cJSON_CreateObject();
        cJSON_AddItemToArray(movement_array, movement_item);
        cJSON_AddNumberToObject(movement_item, "id", f->fsm_hit[i].u8_index);
        cJSON_AddStringToObject(movement_item, "status", is_active_system(this) ? "on" : "off");
        cJSON_AddStringToObject(movement_item, "movement", accel_get_flag_movement (i) ? "yes" : "no");
        fsm_hit_detection_get_xyz(&(f->fsm_hit[i]), &x, &y, &z);
        cJSON_AddNumberToObject(movement_item, "x", (int)x);
        cJSON_AddNumberToObject(movement_item, "y", (int)y);
        cJSON_AddNumberToObject(movement_item, "z", (int)z);
    }

    //LIGHT
    cJSON* light = cJSON_CreateObject();
    cJSON* light_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "light", light_array);
    for (int i = 0; i < f->n_leds; i++) {
        cJSON* light_item = cJSON_CreateObject();
        cJSON_AddItemToArray(light_array, light_item);
        cJSON_AddNumberToObject(light_item, "id", f->fsm_led[i].u8_index);
        cJSON_AddStringToObject(light_item, "status", is_active_system(this) ? "on" : "off");
    }

    char* json_data = cJSON_Print(root);
    // printf("%s\n", json_data);

    mqtt_hal_publish(&(f->client), MQTT_PUBLISH_TOPIC, json_data);
    f->timeout = get_tick_count() + f->sampling_ms;

    cJSON_Delete(root);
    cJSON_Delete(door);
    cJSON_Delete(hit);
    cJSON_Delete(movement);
    cJSON_Delete(light);
    os_free(json_data);
}

static void disable_system(fsm_t* this)
{  
    fsm_send_data_t *f = (fsm_send_data_t*) this;
    
    for (int i = 0; i < f->n_leds; i++) {
        fsm_led_alarm_turn_on_off_active(&(f->fsm_led[i]), 0); 
    }
        for (int i = 0; i < f->n_accel; i++) {
        fsm_hit_detection_set_active(&(f->fsm_hit[i]), 0); 
    }  
    for (int i = 0; i < f->n_doors; i++) {
        fsm_door_checking_turn_on_off_active (&(f->fsm_door[i]), 0); 
    }
}

/* Funciones públicas ********************************************************/
void fsm_send_data_init(fsm_send_data_t* this, uint16_t sampling_ms, fsm_led_alarm_t fsm_led[], uint8_t n_leds, fsm_door_checking_t fsm_door[], uint8_t n_doors, fsm_hit_detection_t fsm_hit[], uint8_t n_accel)
{
    fsm_init ((fsm_t *) this, tabla_trans);
    this->sampling_ms = sampling_ms;
    this->timeout = 0;
    this->n_leds = n_leds;
    this->n_accel = n_accel;
    this->n_doors = n_doors;

    for (int i = 0; i < this->n_leds; i++) {
        this->fsm_led[i] = fsm_led[i];
    }
    for (int i = 0; i < this->n_accel; i++) {
        this->fsm_hit[i] = fsm_hit[i]; 
    }  
    for (int i = 0; i < this->n_doors; i++) {
        this->fsm_door[i] = fsm_door[i]; 
    }
}

void fsm_send_data_set_active (fsm_send_data_t* this, uint8_t val)
{
    this->active = val;
}
