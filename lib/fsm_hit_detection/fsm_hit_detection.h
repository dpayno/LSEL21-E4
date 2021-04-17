#ifndef FSM_HIT_DETECTION_H
#define FSM_HIT_DETECTION_H

#include "fsm.h"
#include <stdint.h>

enum accel_state {
  ACCEL_OFF,
  ACCEL_ON_WAIT,
  ACCEL_ON_RUN,
};

typedef struct
{
  fsm_t fsm_hit_detection;
  uint8_t u8_index;
}fsm_hit_detection_t;

void fsm_hit_detection_init(fsm_hit_detection_t* f, uint16_t threshold_x, uint16_t threshold_y, uint16_t threshold_z, uint16_t sampling, uint8_t pin_SLC, uint8_t pin_SDA);
int fsm_hit_detection_alarm (fsm_hit_detection_t* this);
void activate_system (fsm_hit_detection_t* this, uint8_t val);
static int is_active ( fsm_t* this);
static int is_not_active ( fsm_t* this);
static int is_accel_timeout (fsm_t* this);
static int check_accel_threshold (fsm_t* this);
static int check_no_accel_threshold (fsm_t* this);
static void init_accel_timeout (fsm_t* this);
static void crash_clear (fsm_t* this);
static void get_accel_refresh_timeout(fsm_t* this);
static void crash_set (fsm_t* this);
#endif // FSM_LED_ALARM_H