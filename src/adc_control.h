#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H

#include <inttypes.h>

void adc_enable();
void adc_disable();
uint16_t adc_read(uint8_t mux);

#endif
