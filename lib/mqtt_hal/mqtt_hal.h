#ifndef _MQTT_HAL_H
#define _MQTT_HAL_H

#include "MQTTClient.h"

void mqtt_hal_init(MQTTClient* client, int port, char* broker_addr, char* client_id);

void mqtt_hal_suscribe(MQTTClient* client, char* topic);

void mqtt_hal_publish(MQTTClient* client, char* topic, char* payload);

void mqtt_hal_fire(MQTTClient* client);

#endif // _MQTT_HAL_H