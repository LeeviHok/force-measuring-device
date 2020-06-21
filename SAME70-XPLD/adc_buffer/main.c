#include <atmel_start.h>
#include <stdint.h>     // For fixed size data types
#include <stdio.h>      // For debugging
#include "adc.h"
#include "xdmac.h"
#include "spi.h"

#define ADC_BUFFER_SIZE 100 // Can store 10 s worth of samples @ 10 Hz.

void adc_buffer_init(void);
void adc_buffer_write(int32_t data);
int32_t adc_buffer_read(void);
bool adc_buffer_empty(void);
uint32_t adc_buffer_get_data_count(void);

struct ring_buffer {
    int32_t buffer[ADC_BUFFER_SIZE];
    uint32_t write_offset;
    uint32_t read_offset;
};
struct ring_buffer adc_buff;

volatile bool spi0_tx_done = true;  // Flag to check if SPI0 TX is finished


int main() {
    int32_t dma_buff[ADC_BUFFER_SIZE];  // Flat buffer to store one dma block transfer data

	atmel_start_init();     // Initializes MCU, drivers and middleware
    adc_buffer_init();      // Initialize ring buffer for ADC data
    XDMAC_init(SPI0_TDR);   // Initialize XDMAC for data transfer between DMA buffer and SPI0
    SPI0_init();            // Initialize SPI0 for communication with Raspberry Pi
    adc_init();             // Initialize ADC

	while (1) {
        if(!gpio_get_pin_level(ADC_DOUT)) { // If ADC data is ready
            adc_buffer_write(adc_read());
        }

        if(spi0_tx_done && (!adc_buffer_empty())) {             // If SPI transfer is finished and ring buffer contains samples
            spi0_tx_done = false;                               // Clear spi tx done flag
            uint32_t data_count = adc_buffer_get_data_count();  // Store number of samples in ADC buffer

            uint32_t i;
            for(i = 0; i < data_count; i++) {       // Store all samples from ADC buffer to DMA buffer
                dma_buff[i] = adc_buffer_read();    //
            }

            XDMAC_start(data_count, dma_buff);
            SPI0_start();
        }
	}
}


void adc_buffer_init(void) {
    adc_buff.write_offset = 0;
    adc_buff.read_offset = 0;
}

void adc_buffer_write(int32_t data) {
    if(adc_buff.write_offset >= ADC_BUFFER_SIZE) {  // If write offset is at end of array
        adc_buff.write_offset = 0;                  // Set write offset back to beginning of array
    }

    adc_buff.buffer[adc_buff.write_offset] = data;  // Write to buffer
    adc_buff.write_offset++;                        // Increment write offset
}

int32_t adc_buffer_read(void) {
    int32_t value;

    if(adc_buffer_empty()) {                            // If buffer is empty
        printf("ERROR: ADC buffer is empty");           // Print error message
        while(1);                                       // Get stuck here
    }
    else if(adc_buff.read_offset >= ADC_BUFFER_SIZE) {  // If read offset is not at end of array
        adc_buff.read_offset = 0;                       // Set read offset back to beginning of array
    }

    value = adc_buff.buffer[adc_buff.read_offset];      // Read from buffer
    adc_buff.read_offset++;                             // Increment read offset

    return value;
}

bool adc_buffer_empty(void) {
    if(adc_buff.write_offset == adc_buff.read_offset) {
        return true;
    }
    else {
        return false;
    }
}

uint32_t adc_buffer_get_data_count(void) {
    if(adc_buff.write_offset < adc_buff.read_offset) {                          // If write offset is wrapped back to
        return adc_buff.write_offset + ADC_BUFFER_SIZE - adc_buff.read_offset;  // beginning but read offset is not
    }
    else {
        return adc_buff.write_offset - adc_buff.read_offset;
    }
}

void XDMAC_Handler(void) {
    *XDMAC_CIS0;    // Clear interrupts
}

void SPI0_Handler(void) {
    gpio_set_pin_level(SPI0_DRDY, false);   // Clear data ready
    spi0_tx_done = true;                    // Set SPI tx done flag
    NVIC_DisableIRQ(SPI0_IRQn);             // Disable SPI0 NVIC interrupts
}
