#include "motor_driver.h"
#include "uart_com.h"
#include "adc_control.h"
// #include "LCD_I2C.h"
// #include "i2c.h"

#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

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

	// i2c_init();
	// i2c_start();
	// i2c_write(0x70);
	// lcd_init();

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
		uart_printf("%d\n", rpm);
		// lcd_cmd(0x80);
		// lcd_msg("dupa");
	}
}
