#ifndef _WEBSOCKET_H
#define _WEBSOCKET_H

#include <stdint.h>

void websocket_start(void *optarg);

int8_t websocket_stop(void);

#endif // _WEBSOCKET_H