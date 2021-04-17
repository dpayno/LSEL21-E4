#include "accelerometer.h"
#include "fsm_hit_detection.h"
#include "timer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

/* Tabla de transiciones *****************************************************************/
static fsm_trans_t tabla_trans[] = {
	  { ACCEL_OFF,     is_active,                ACCEL_ON_WAIT, init_accel_timeout        },
	  { ACCEL_ON_WAIT, is_not_active,            ACCEL_OFF,     crash_clear               },
	  { ACCEL_ON_WAIT, is_accel_timeout,         ACCEL_ON_RUN,  get_accel_refresh_timeout },
      { ACCEL_ON_RUN,  check_accel_threshold,    ACCEL_ON_WAIT, crash_set                 },
      { ACCEL_ON_RUN,  check_no_accel_threshold, ACCEL_ON_WAIT, NULL                      },
	  { -1,            NULL,                     -1,            NULL                      },
};


void fsm_hit_detection_init(fsm_hit_detection_t* this, uint16_t threshold_x, uint16_t threshold_y, uint16_t threshold_z, uint16_t sampling, uint8_t pin_SLC, uint8_t pin_SDA)
{
    fsm_init ((fsm_t *) this, tabla_trans);
    this->u8_index = get_new_index();
    accel_set_thresholds( this->u8_index, threshold_x, threshold_y, threshold_z);
    accel_set_pins( this->u8_index, pin_SDA, pin_SLC);
    accel_set_sampling( this->u8_index, ms_into_ticks(sampling));
}


 static int is_active ( fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    return accel_is_active( f->u8_index );
}

static int is_not_active ( fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    return !accel_is_active( f->u8_index );
}

static int is_accel_timeout (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    return (get_tick_count() >= accel_get_timeout( f->u8_index) && accel_is_active( f->u8_index ));
}

static int check_accel_threshold (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    return accel_check_accel_xyz (f->u8_index);
}

static int check_no_accel_threshold (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    return !accel_check_accel_xyz (f->u8_index);
}

static void init_accel_timeout (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    accel_set_timeout (f->u8_index, get_tick_count() + accel_get_sampling( f->u8_index));
}

static void crash_clear (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    accel_set_flag_crash( f->u8_index, 0);
}

static void get_accel_refresh_timeout(fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;

    /*LEER DATOS DEL ACELERÓMETRO*/

    accel_get_accel_xyz ( f->u8_index, 1, 1, 101 ); /*ALMACENAR DATOS DEL ACELERÓMETRO PARA COMPARAR DESPUÉS*/
    accel_set_timeout (f->u8_index, get_tick_count() + accel_get_sampling( f->u8_index)); 
}

static void crash_set (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    accel_set_flag_crash( f->u8_index, 1);
}

int fsm_hit_detection_alarm (fsm_hit_detection_t* this)
{
    return accel_get_flag_crash(this->u8_index);
}

void activate_system (fsm_hit_detection_t* this, uint8_t val)
{
    accel_set_active(this->u8_index, val);    
}