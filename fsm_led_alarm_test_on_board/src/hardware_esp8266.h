#ifndef HARDWARE_ESP8266_H
#define HARDWARE_ESP8266_H

#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "stdint.h"

#define D0 16
#define D1 5 //GPIO_Pin_5
#define D2 4 //GPIO_Pin_4
#define D3 0 //GPIO_Pin_0
#define D4 2 //GPIO_Pin_2
#define D5 14 //GPIO_Pin_14
#define D6 12 //GPIO_Pin_12
#define D7 13 //GPIO_Pin_13
#define D8 15 //GPIO_Pin_15 

enum hard_gpio_mode {
    HARD_GPIO_OUTPUT,
    HARD_GPIO_INPUT,
    HARD_GPIO_INPUT_PULLUP,
    HARD_GPIO_INTR
};

void hard_init (uint8_t);

void hard_gpio_pinSetUp (uint16_t, uint8_t);

void hard_gpio_digitalWrite (uint16_t, uint8_t);

uint8_t hard_gpio_digitalRead (uint16_t);


#endif