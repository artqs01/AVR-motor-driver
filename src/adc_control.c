#include "adc_control.h"

#include <avr/io.h>

void adc_enable()
{
	//Enable ADC
	ADCSRA = (1 << ADEN);
}

void adc_disable()
{
	//Disable ADC
	ADCSRA &= ~(1 << ADEN);
}

uint16_t adc_read(uint8_t mux)
{
	//Read data from selected ADC (VCC as reference volatge)
	ADMUX = (mux << MUX0) | (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}
