#include "fsm_hit_detection.h"

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "accel_driver.h"
#include "timer.h"

/* Definición funciones privadas *********************************************/
// Funciones de guarda
static int is_active ( fsm_t* this);
static int is_not_active ( fsm_t* this);
static int is_accel_timeout (fsm_t* this);
static int check_accel_threshold_hard (fsm_t* this);
static int check_accel_threshold_soft (fsm_t* this);
static int check_no_accel_threshold (fsm_t* this);
// Funciones de salida
static void init_accel_timeout (fsm_t* this);
static void crash_movement_clear (fsm_t* this);
static void movement_clear (fsm_t* this);
static void get_accel_refresh_timeout(fsm_t* this);
static void crash_set (fsm_t* this);
static void movement_set (fsm_t* this);

/* Tabla de transiciones *****************************************************/
static fsm_trans_t tabla_trans[] = {
    { ACCEL_OFF,     is_active,                  ACCEL_ON_WAIT, init_accel_timeout        },
    { ACCEL_ON_WAIT, is_not_active,              ACCEL_OFF,     crash_movement_clear      },
    { ACCEL_ON_WAIT, is_accel_timeout,           ACCEL_ON_RUN,  get_accel_refresh_timeout },
    { ACCEL_ON_RUN,  check_accel_threshold_hard, ACCEL_ON_WAIT, movement_set              },
    { ACCEL_ON_RUN,  check_accel_threshold_soft, ACCEL_ON_WAIT, crash_set                 },
    { ACCEL_ON_RUN,  check_no_accel_threshold,   ACCEL_ON_WAIT, NULL                      },
    { -1,            NULL,                       -1,            NULL                      },
};

/* Implementación funciones privadas *****************************************/
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

static int check_accel_threshold_hard (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    return accel_check_accel_hard_xyz (f->u8_index);
}

static int check_accel_threshold_soft (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    return accel_check_accel_soft_xyz (f->u8_index);
}

static int check_no_accel_threshold (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    return !accel_check_accel_soft_xyz (f->u8_index);
}

static void init_accel_timeout (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    uint8_t pin_SDA, pin_SCL;
    uint16_t x, y, z;

    accel_get_pins(f->u8_index, &pin_SDA, &pin_SCL);
    accel_set_timeout (f->u8_index, get_tick_count() + accel_get_sampling( f->u8_index));
    accel_driver_init(pin_SDA, pin_SCL);
    accel_driver_get_xyz(&x, &y, &z); // Lectura ciega
    accel_set_accel_xyz (f->u8_index, x, y, z);
    printf("x: %d, y: %d, z: %d\n", x, y, z);
}

static void crash_movement_clear (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;

    accel_set_flag_crash( f->u8_index, 0);
    accel_set_flag_movement( f->u8_index, 0);
}

static void get_accel_refresh_timeout(fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    uint16_t x, y, z;

    accel_driver_get_xyz(&x, &y, &z);
    accel_set_accel_xyz ( f->u8_index, x, y, z);
    accel_set_timeout (f->u8_index, get_tick_count() + accel_get_sampling( f->u8_index));
    // printf("x: %d, y: %d, z: %d\n", x, y, z);
}

static void crash_set (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    accel_set_flag_crash( f->u8_index, 1);
    printf("Crash detected!\n");
}

static void movement_set (fsm_t* this)
{
    fsm_hit_detection_t* f = (fsm_hit_detection_t*)this;
    accel_set_flag_movement( f->u8_index, 1);
    printf("Movement detected!\n");
}

/* Funciones públicas ********************************************************/
void fsm_hit_detection_init(fsm_hit_detection_t* this, accel_threshold_t hard, accel_threshold_t soft, uint16_t sampling, uint8_t pin_SLC, uint8_t pin_SDA)
{
    fsm_init ((fsm_t *) this, tabla_trans);
    this->u8_index = accel_get_new_index();
    accel_set_thresholds( this->u8_index, hard, soft);
    accel_set_pins( this->u8_index, pin_SDA, pin_SLC);
    accel_set_sampling( this->u8_index, ms_into_ticks(sampling));
}

int fsm_hit_detection_alarm (fsm_hit_detection_t* this)
{
    return accel_get_flag_crash(this->u8_index);
}

void fsm_hit_detection_set_active (fsm_hit_detection_t* this, uint8_t val)
{
    accel_set_active(this->u8_index, val);    
}

void fsm_hit_detection_get_xyz (fsm_hit_detection_t* this, uint16_t* x, uint16_t* y, uint16_t* z)
{
    accel_get_accel_xyz (this->u8_index, x, y, z);
}