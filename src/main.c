#include "motor_driver.h"
#include "uart_com.h"
#include "adc_control.h"

#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

volatile uint8_t adc_read_flag = 0;
volatile uint8_t print_flag = 0;

ISR(TIMER1_COMPA_vect)
{
	adc_read_flag = 1;
	print_flag = 1;
}

void print_motor_data(uint16_t rpm, uint8_t if_ccw)
{
	if (rpm < 10)
		uart_printf("  %d,%c", rpm, (if_ccw) ? 'L' : 'R');
	else if (rpm > 100)
		uart_printf("%d,%c", rpm, (if_ccw) ? 'L' : 'R');
	else
	 uart_printf(" %d,%c", rpm, (if_ccw) ? 'L' : 'R');
}

int main()
{
	driver_init();
	uart_init();
	adc_enable();

	sei();
	
	uint16_t rpm = 0;
	motor_control m_ctl = {0, 0, 0, &OCR0A};
	driver_motor_enable();
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
			m_ctl.pwr = (adc_read(0) >> 8) * 0.6;
			driver_power_adjust(&m_ctl);
			driver_rpm_update(&rpm, &m_ctl);
		}
		if (print_flag) {
			print_flag = 0;
			print_motor_data(rpm, m_ctl.if_ccw);
		}
	}
}
