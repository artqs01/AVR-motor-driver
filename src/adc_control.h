#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H

#include <inttypes.h>

void adcenable();
void adcdisable();
uint16_t adcread(uint8_t mux);

#endif
