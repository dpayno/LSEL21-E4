#include "timer.h"

#include "esp_common.h"
#include "freertos/task.h"

uint32_t timer_get_tick_count ()
{
    return xTaskGetTickCount();
}

uint32_t timer_ms_to_ticks (uint32_t val)
{
    return val/portTICK_RATE_MS;
}