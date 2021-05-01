#include "mqtt_hal.h"

static Network network;
static unsigned char sendbuf[80], readbuf[80];
static int rc = 0;
static MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

void mqtt_message_arrived(MessageData* data)
{
    int len = (int)data->message->payloadlen;
    char rx_msg[len];
    memcpy(rx_msg, data->message->payload, len);
    printf("Received msg: %s\n", rx_msg);
    printf("Size of data: %d\n",(int)data->message->payloadlen );
    // fsm_send_data_set_active(&fsm_send, 1);
}

void mqtt_hal_init(MQTTClient* client, int port, char* broker_addr, char* client_id)
{
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

    if ((rc = MQTTPublish(client, topic, &message)) != 0)
        printf("Return code from MQTT publish is %d\n", rc);

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