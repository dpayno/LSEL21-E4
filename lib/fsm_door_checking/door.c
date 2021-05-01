#include "door.h"

static flags_door_t flags_doors[N_DOORS];
static uint8_t m_u8_index = 0;

uint8_t door_get_new_index ()
{
  flags_doors[m_u8_index].data = 0x00;
  return m_u8_index++;
}

void door_set_active (uint8_t index, uint8_t val)
{
  flags_doors[index].active = val;
}

uint8_t door_is_active (uint8_t index)
{
  return flags_doors[index].active;
}

void door_set_flag_door_open (uint8_t index, uint8_t val)
{
  flags_doors[index].flag_door_open = val;
}

uint8_t door_get_flag_door_open (uint8_t index)
{
  return flags_doors[index].flag_door_open;
}

uint8_t door_is_door_open (uint8_t index)
{
  flags_doors[index].door_status = hard_gpio_digitalRead (flags_doors[index].pin);
  return flags_doors[index].door_status;
}

void door_set_pin (uint8_t index, uint8_t pin)
{
  hard_gpio_pinSetUp (pin, HARD_GPIO_INPUT);
  flags_doors[index].pin = pin;
}