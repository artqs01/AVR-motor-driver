#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

//port b
#define L_EN 1
#define R_IS 0
#define L_IS 2

// port d
#define R_EN 7
#define PWM 6

#define PCT 20

typedef struct motor_control
{
	uint8_t adjust_pwr_cnt;
	uint8_t pwr;
} motor_control;

void driver_init();
void driver_enable();
void driver_disable();
void driver_power_adjust(motor_control* m_ctl);

#endif
