#include "mqtt_hal.h"
#include "cJSON.h"
#include <stdlib.h>

static Network network;
static unsigned char sendbuf[512], readbuf[128];
static int rc = 0;
static MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
static int count = 0;
static fsm_send_data_t* p_fsm_send;

void mqtt_message_arrived(MessageData* data)
{
    // Get RAW MQTT message
    int len = (int)data->message->payloadlen;
    char rx_msg[len];
    memcpy(rx_msg, data->message->payload, len);
    // Parse message to JSON
    cJSON* rx_json = cJSON_Parse(rx_msg);
    // Check if it's a JSON message
    if (!rx_json) {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    } else {
        // Obtain "active" value & convert to integer
        cJSON* active_json = cJSON_GetObjectItem(rx_json, "active");
        char* active_str = cJSON_Print(active_json);
        printf("New active value: %s\n", active_str);
        int active_int = atoi(active_str);
        // Free memory
        cJSON_Delete(rx_json);
        os_free(active_str);
        // Active / deactive system
        fsm_send_data_set_active(p_fsm_send, active_int);
    }
}

void mqtt_hal_init(MQTTClient* client, int port, char* broker_addr, char* client_id, fsm_send_data_t* fsm_send)
{
    p_fsm_send = fsm_send;
    NetworkInit(&network);
    MQTTClientInit(client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = ConnectNetwork(&network, broker_addr, port)) != 0)
        printf("Return code from network connect is %d\n", rc);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = client_id;

    if ((rc = MQTTConnect(client, &connectData)) != 0)
        printf("Return code from MQTT connect is %d\n", rc);
    else
        printf("MQTT Connected\n");
}

void mqtt_hal_suscribe(MQTTClient* client, char* topic)
{
    if ((rc = MQTTSubscribe(client, topic, 2, mqtt_message_arrived)) != 0)
        printf("Return code from MQTT subscribe is %d\n", rc);
}

void mqtt_hal_publish(MQTTClient* client, char* topic, char* payload)
{
    rc = 0;
    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    if ((rc = MQTTPublish(client, topic, &message)) != 0) {
        printf("Return code from MQTT publish is %d\n", rc);
    } else {
        printf("Sensor data sent! Count: %d\n", count++);
    }
    if ((rc = MQTTYield(client, 1000)) != 0)
        printf("Return code from yield is %d\n", rc);
}

void mqtt_hal_fire(MQTTClient* client)
{
#if !defined(MQTT_TASK)
    if ((rc = MQTTYield(client, 1000)) != 0)
        printf("Return code from yield is %d\n", rc);
#endif
}