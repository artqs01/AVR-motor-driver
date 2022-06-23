#include "motor_driver.h"

#include <avr/interrupt.h>
#include <util/atomic.h>

void driver_init()
{
	// Timer 0 set as PWM
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
	TCCR0B =  (1 << CS01);
	OCR0A = 0;
	
	// Timer 1 set as CTC to update motor velocity
	TCCR1A = 0;
	TCCR1B =  (1 << CS10) | (1 << CS12) | (1 << WGM12);
	OCR1A = 200;
	TIMSK1 = (1 << OCIE1A);

	// driver outputs and inputs
	DDRD |= (1 << R_EN) | (1 << PWM);
	DDRB |= (1 << L_EN);
	DDRB &= ~((1 << L_IS) | (1 << R_IS));

	DDRC &= ~(1 << 0);
}
void driver_enable()
{
	PORTB |= (1 << L_EN);
	PORTD |= (1 << R_EN);
}

void driver_disable()
{
	PORTB &= ~(1 << L_EN);
	PORTD &= ~(1 << R_EN);
}

void driver_power_adjust(motor_control* m_ctl)
{
	if (++m_ctl->adjust_pwr_cnt == PCT)
	{
		if (OCR0A < m_ctl->pwr)
			OCR0A += 1;
		else if (OCR0A > m_ctl->pwr)
			OCR0A -= 1;
		m_ctl->adjust_pwr_cnt = 0;
	}
}
