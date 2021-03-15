#include "door.h"

typedef union {
  struct
  {
    unsigned active:1;
    unsigned door_status:1;
    unsigned flag_door_open:1;
    unsigned pin:5;
  };
  uint8_t data;
}flags_door_t;

static flags_door_t flags_doors[NDoors];
static uint8_t m_u8_index = 0;

uint8_t get_new_index ()
{
  flags_doors[m_u8_index].data = 0x00;
  return m_u8_index++;
}

void set_active (uint8_t index, uint8_t val)
{
  flags_doors[index].active = val;
}

uint8_t is_active (uint8_t index)
{
  return flags_doors[index].active;
}

void set_flag_door_open (uint8_t index, uint8_t val)
{
flags_doors[index].flag_door_open = val;
}

uint8_t get_flag_door_open (uint8_t index)
{
  return flags_doors[index].flag_door_open;
}

uint8_t is_door_open (uint8_t index)
{
  flags_doors[index].door_status = hard_gpio_digitalRead (flags_doors[index].pin);
  return flags_doors[index].door_status;
}

void set_pin (uint8_t index, uint8_t pin)
{
  hard_gpio_pinSetUp (pin, HARD_GPIO_INPUT);
  flags_doors[index].pin = pin;
}