#ifndef HARDWARE_H
#define HARDWARE_H

#include "stdint.h"

enum boards {
    BOARD_ESP8266,
};

enum hard_gpio_mode {
    HARD_GPIO_OUTPUT,
    HARD_GPIO_INPUT,
    HARD_GPIO_INPUT_PULLUP,
    HARD_GPIO_INTR
};

uint8_t m_u8_hard_board_type = BOARD_ESP8266;


void hard_gpio_pinSetUp (uint16_t);

void hard_gpio_digitalWrite (uint16_t, uint8_t);

uint8_t hard_gpio_digitalRead (uint16_t);


#endif