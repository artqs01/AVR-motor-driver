#include "adc_control.h"

#include <avr/io.h>

void adc_enable()
{
	ADCSRA = (1 << ADEN);
}

void adc_disable()
{
	ADCSRA &= ~(1 << ADEN);
}

uint16_t adc_read(uint8_t mux)
{
	//Read data from selected ADC with VCC as reference voltage
	ADMUX = (mux << MUX0) | (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}
