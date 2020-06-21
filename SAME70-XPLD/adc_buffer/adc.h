#include <stdint.h> // For fixed size data types


void adc_init(void);    // Powers up ADC and waits until data is ready
int32_t adc_read(void); // Reads one sample from ADC
