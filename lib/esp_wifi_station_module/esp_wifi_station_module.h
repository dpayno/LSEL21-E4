#ifndef ESP_WIFI_STATION_MODULE_H
#define ESP_WIFI_STATION_MODULE_H

#include "c_types.h"
#include <string.h>
#include "lwip/ip_addr.h"
#include "esp_wifi.h"
#include "esp_sta.h"

typedef struct esp_wifi_station_module {
  struct station_config esp;
} esp_wifi_station_module;

/**
  * @brief  Initialize WiFi connection betwen a ESP8266 in station mode and an AP. Make sure initialize a WiFi Station Module first.
  *
  * @param     esp_wifi_station_module* mod       : station module.
  * @param     char*  ssid                        : AP name.
  * @param     char*  password                    : AP password name.
  *
  * @return    void             
  */
void init_module_station(esp_wifi_station_module* mod, char* ssid, char* password);

/**
  * @brief  Make connection with AP
  *
  * @param     void
  *
  * @return    void
  */
void connect_to_ap(void);

/**
  * @brief  Check if connection is ok.
  *
  * @param     void
  *
  * @return    true  : connected
  * @return    false : not connected 
  */
bool station_connected(void);

/**
  * @brief  Check if operation modeis correct.
  *
  * @param     void
  *
  * @return    true  : correct
  * @return    false : not correct 
  */
bool check_station_operation_mode(void);

#endif 