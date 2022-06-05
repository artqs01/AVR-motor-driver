#include "motor_driver.h"

#include <avr/interrupt.h>
#include <util/atomic.h>

void driver_init()
{
	// Timer 0 set as PWM
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
	TCCR0B =  (1 << CS00);
	OCR0A = 0;
	
	// Timer 1 set as CTC to update motor velocity
	TCCR1A = 0;
	TCCR1B =  (1 << CS10) | (1 << CS12) | (1 << WGM12);
	OCR1A = 200;
	TIMSK1 = (1 << OCIE1A);

	// driver outputs and inputs
	DDRD |= (1 << R_EN) | (1 << PWM);
	DDRB |= (1 << L_EN) | (1 << L_IS) | (1 << R_IS);
	PORTB |= (1 << L_EN);
	PORTD &= ~(1 << R_EN);
	PORTB &= ~((1 << R_IS) | (1 << L_IS));
	DDRC &= ~(1 << 0);
}
