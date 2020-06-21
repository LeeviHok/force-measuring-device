#include <atmel_start.h>
#include <stdint.h>     // For fixed size data types
#include "adc.h"

/*
 * Initializes ADC by exiting power-down mode
 * and waits until first data is ready.
 */
void adc_init(void) {
    gpio_set_pin_level(ADC_SCK, false);     // Exit power-down mode
    while(!gpio_get_pin_level(ADC_DOUT));   // Wait until ADC data is ready
}

/*
 * Bit banged communication for tricky ADC interface.
 *
 * ADC uses two pins to transmit data to MCU, CLK and DOUT.
 * CLK pin is also used to control power-down, gain and channel selection.
 * Power-down is enabled by holding CLK HIGH over 60 us and disabled by
 * dropping CLK back to LOW.
 * Gain and channel is selected by providing additional CLK pulses after
 * all 24 bits of data has been read.
 *
 * 25 CLK: Channel A, 128 gain
 * 26 CLK: Channel B, 32 gain
 * 27 CLK: Channel A, 64 gain
 */
int32_t adc_read(void) {
    uint32_t sample = 0;                    // To store ADC 24 bit sample

    gpio_set_pin_level(ADC_SCK, false);     // Power up ADC (if in power down mode)
    while(gpio_get_pin_level(ADC_DOUT));    // Wait as long as ADC data is not ready (DOUT is high)
    delay_us(1);                            // Minimum of 100 ns delay after DOUT drops low

    for(int i = 0; i < 24; i++) {
        gpio_toggle_pin_level(ADC_SCK);     // SCK rising edge
        delay_us(1);                        // Minimum of 100 ns delay from SCK rising edge to data ready
                                            // Serves also as minimum of 200 ns SCK high time

        sample <<= 1;                       // Shift bits to left by one
        if(gpio_get_pin_level(ADC_DOUT)) {
            sample++;                       // Set first bit
        }

        gpio_toggle_pin_level(ADC_SCK);     // SCK falling edge
        delay_us(1);                        // Minimum of 200 ns SCK low time
    }

    /* Generate one extra clock pulse to keep gain at 128 */
    gpio_toggle_pin_level(ADC_SCK); // SCK rising edge
    delay_us(1);                    // Minimum of 200 ns SCK high time
    gpio_toggle_pin_level(ADC_SCK); // SCK falling edge

    /* Sign extend to 32 bits */
    if(sample & (1 << 23)) {
        sample |= 0xFF000000;
    }
    else {
        sample &= ~0xFF000000;
    }

    return (int32_t) sample;
}
