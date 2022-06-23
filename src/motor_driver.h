#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

//port b
#define L_EN 1
#define R_IS 0
#define L_IS 2

// port d
#define R_EN 7
#define L_PWM 5
#define R_PWM 6

#define DIR_SW (!!(PIND & (1 << 2)))

#define PCT 10

typedef struct motor_control
{
	int16_t adjust_pwr_cnt;
	uint8_t pwr;
	uint8_t if_ccw;
	volatile uint8_t* control_register;
} motor_control;

void driver_init();
void driver_motor_enable();
void driver_motor_disable();
void driver_change_direction(motor_control* m_ctl);
void driver_power_adjust(motor_control* m_ctl);

#endif
