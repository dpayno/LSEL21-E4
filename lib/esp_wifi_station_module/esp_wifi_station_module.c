#include "esp_wifi_station_module.h"

void init_module_station(esp_wifi_station_module* mod, char* ssid, char* password)
{
    wifi_set_opmode_current(STATION_MODE);
    strcpy((char*)(mod->esp.ssid), ssid);
    strcpy((char*)(mod->esp.password), password);
    wifi_station_set_config_current(&(mod->esp));
}

void connect_to_ap(void)
{
  wifi_station_connect();
}

bool station_connected(void)
{
  return (wifi_station_get_connect_status() == STATION_GOT_IP);
}

bool check_station_operation_mode(void)
{
  return (wifi_get_opmode_default() == STATION_MODE);
}