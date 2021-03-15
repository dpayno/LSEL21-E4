#include "hardware_esp8266.h"

void hard_gpio_pinSetUp (uint16_t pin, uint8_t mode) 
{  
    switch (mode)
    {
    case HARD_GPIO_OUTPUT:
        if (pin == D0) gpio16_output_conf();
        else GPIO_AS_OUTPUT(0x01<<pin);
        break;

    case HARD_GPIO_INPUT:
        if (pin == D0) gpio16_input_conf();
        else GPIO_AS_INPUT(0x01<<pin);
        break;
    
    default:
        break;
    }
}

void hard_gpio_digitalWrite (uint16_t pin, uint8_t val)
{
    if (pin == D0) gpio16_output_set(val);
    else GPIO_OUTPUT(0x01<<pin, val);
}

uint8_t hard_gpio_digitalRead (uint16_t pin) {
    if (pin == D0) return gpio16_input_get();
    else return GPIO_INPUT_GET(pin);
}