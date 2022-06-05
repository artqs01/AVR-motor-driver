#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "motor_driver.h"
#include "uart_com.h"
#include "adc_control.h"

ISR(TIMER1_COMPA_vect)
{
	OCR0A = (adcread(0) >> 8);
}

int main()
{
	
	driver_init();
	adcenable();
	
	OCR0A = 0;
	sei();
	while (1)
	{
		_delay_ms(20);
	}
}
