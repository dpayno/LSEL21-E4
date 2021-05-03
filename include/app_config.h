#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#define AP_SSID               "your_wifi"
#define AP_PASSWORD           "your_password"

#define MQTT_BROKER           "192.168.0.110"
#define MQTT_CLIENT           "esp8266_client_example"
#define MQTT_PORT             8883
#define MQTT_SUSCRIBE_TOPIC   "alarm_status"
#define MQTT_SEND_RATE_MS     10000UL

#define LED_ALARM_RATE_MS     500UL
#define LED_ALARM_PIN         D1

#define DOOR_CHECKING_PIN     D2

#define ACCEL_SDA_PIN         D4
#define ACCEL_SCL_PIN         D3
#define ACCEL_THRESHOLD_HARD  0xFFFF >> 2
#define ACCEL_THRESHOLD_SOFT  0xFFFF >> 3
#define ACCEL_RATE_MS         200UL

#endif // APP_CONFIG_H