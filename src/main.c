#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#include "motor_driver.h"
#include "uart_com.h"
#include "adc_control.h"

volatile uint8_t adc_read_flag = 0;

ISR(TIMER1_COMPA_vect)
{
	adc_read_flag = 1;
}

int main()
{
	driver_init();
	uart_init();
	adc_enable();
	sei();

	motor_control m_ctl = {0, 0, 0, &OCR0A};

	// DDRD = (1 << 6);

	driver_enable();
	while (1)
	{
		if (DIR_SW)
		{
			while (DIR_SW);
			driver_change_direction(&m_ctl);
		}
		if (adc_read_flag)
		{
			adc_read_flag = 0;
			m_ctl.pwr = (adc_read(0) >> 8);
			driver_power_adjust(&m_ctl);
		}
		// PORTD ^= (1 << 6);
		// _delay_ms(500);
	}
}
