#include "motor_driver.h"
#include "adc_control.h"
#include "uart_com.h"

#include <avr/interrupt.h>
#include <util/delay.h>

void driver_init()
{
	// Timer 0 set as PWM and direction set as cw
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
	TCCR0B =  (1 << CS01);
	OCR0A = 0;
	
	// Timer 1 set as CTC to update motor velocity
	TCCR1A = 0;
	TCCR1B = (1 << CS10) | (1 << CS12) | (1 << WGM12);
	OCR1A = 200;
	TIMSK1 = (1 << OCIE1A);

	// driver outputs and inputs
	DDRD |= (1 << R_EN) | (1 << R_PWM) | (1 << L_PWM) | (1 << DIR_SW);
	DDRB |= (1 << L_EN);
	DDRB &= ~((1 << L_IS) | (1 << R_IS));

	DDRC &= ~(1 << 0);
}

void driver_motor_enable()
{
	PORTB |= (1 << L_EN);
	PORTD |= (1 << R_EN);
}

void driver_motor_disable()
{
	PORTB &= ~(1 << L_EN);
	PORTD &= ~(1 << R_EN);
}

void driver_change_direction(motor_control* m_ctl)
{
	m_ctl->if_ccw ^= 1;
	adc_disable();
	m_ctl->pwr = 0;
	*m_ctl->control_register = 0;
	_delay_ms(2000);
	if (m_ctl->if_ccw)
	{
		TCCR0A &= ~(1 << COM0A1);
		TCCR0A |= (1 << COM0B1);
		m_ctl->control_register = &OCR0B;
	}
	else
	{
		TCCR0A &= ~(1 << COM0B1);
		TCCR0A |= (1 << COM0A1);
		m_ctl->control_register = &OCR0A;
	}
	adc_enable();
}

void driver_power_adjust(motor_control* m_ctl)
{
	if (++m_ctl->adjust_pwr_cnt == PCT)
	{
		if (*m_ctl->control_register < m_ctl->pwr)
			*m_ctl->control_register += 1;
		else if (*m_ctl->control_register > m_ctl->pwr)
			*m_ctl->control_register -= 1;
		m_ctl->adjust_pwr_cnt = 0;
	}
}

void driver_rpm_update(uint16_t* rpm, const motor_control* m_ctl)
{
	*rpm = 2750.f * (float)*m_ctl->control_register / 10.26f / 255.f;
}
