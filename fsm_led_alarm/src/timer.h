#ifndef _TIMER_H
#define _TIMER_H

//#include "esp_common.h"
//#include "freertos/task.h"
#include <stdint.h>

uint32_t get_tick_count ();

uint32_t ms_into_ticks (uint32_t);

#endif // _TIMER_H