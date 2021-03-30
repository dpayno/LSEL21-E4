#ifndef ESP_WIFI_STATION_MODULE_H
#define ESP_WIFI_STATION_MODULE_H

#include "esp_sta.h"
#include "esp_wifi.h"
#include <string.h>


typedef struct esp_wifi_station_module{

    struct station_config esp;
}esp_wifi_station_module;



/**
  * @brief  Initialize WiFi connection betwen a ESP8266 in station mode and an AP. Make sure initialize a WiFi Station Module first.
  *
  * @param     esp_wifi_station_module* mod       : station module.
  * @param     char*  ssid                        : AP name.
  * @param     char*  password                    : AP password name.
  *
  * @return    void             
  */
void init_module_station(esp_wifi_station_module* mod, char* ssid, char* password){

    wifi_set_opmode_current(STATION_MODE);
    strcpy((char*)(mod->esp.ssid), ssid);
    strcpy((char*)(mod->esp.password), password);
    wifi_station_set_config_current(&(mod->esp));
}


/**
  * @brief  Make connection with AP
  *
  * @param     void
  *
  * @return    void
  */
void connect_to_ap(void) {wifi_station_connect();}


/**
  * @brief  Check if connection is ok.
  *
  * @param     void
  *
  * @return    true  : connected
  * @return    false : not connected 
  */
bool station_connected(void) {if (wifi_station_get_connect_status() == STATION_GOT_IP) return 1; else return 0;}



/**
  * @brief  Check if operation modeis correct.
  *
  * @param     void
  *
  * @return    true  : correct
  * @return    false : not correct 
  */
bool check_station_operation_mode(void) {if(wifi_get_opmode_default() == STATION_MODE) return 1; else return 0;}



#endif 