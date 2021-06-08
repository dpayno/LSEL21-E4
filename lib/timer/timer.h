#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>

uint32_t timer_get_tick_count ();

uint32_t timer_ms_to_ticks (uint32_t val);

#endif // _TIMER_H