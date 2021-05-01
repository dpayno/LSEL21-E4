#ifndef FSM_HIT_DETECTION_H
#define FSM_HIT_DETECTION_H

#include "fsm.h"
#include <stdint.h>
#include "accelerometer.h"

enum accel_state {
  ACCEL_OFF,
  ACCEL_ON_WAIT,
  ACCEL_ON_RUN,
};

typedef struct {
  fsm_t fsm_hit_detection;
  uint8_t u8_index;
} fsm_hit_detection_t;

void fsm_hit_detection_init(fsm_hit_detection_t* this, accel_threshold_t hard, accel_threshold_t soft, uint16_t sampling, uint8_t pin_SLC, uint8_t pin_SDA);
int fsm_hit_detection_alarm (fsm_hit_detection_t* this);
void fsm_hit_detection_set_active (fsm_hit_detection_t* this, uint8_t val);

#endif // FSM_LED_ALARM_H