#include "timer.h"

#include "esp_common.h"
#include "freertos/task.h"

uint32_t get_tick_count () 
{
    return xTaskGetTickCount();
}

uint32_t ms_into_ticks (uint32_t val)
{
    return val/portTICK_RATE_MS;
}