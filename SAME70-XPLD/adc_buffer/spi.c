#include <atmel_start.h>
#include "spi.h"


void SPI0_init(void) {
    *SPI_SR;                                                    // Clear pending interrupts
    if(*SPI0_WPMR & (1 << WPITEN)) {                            // If interrupt registers write protection is set
        *SPI0_WPMR ^= (PASSWD << WPKEY) | (1 << WPITEN);        // Clear SPI0 write protection for interrupt registers
    }
    if(*SPI0_WPMR & (1 << WPCREN)) {                            // If control register write protection is set
        *SPI0_WPMR ^= (PASSWD << WPKEY) | (1 << WPCREN);        // Clear SPI0 write protection for control register
    }
    *SPI_IDR |= (1 << RDRF) | (1 << TDRE) | (1 << MODF) |       // Disable all SPI interrupts
                (1 << OVRES) | (1 << NSSR) | (1 << TXEMPTY) |   //
                (1 << UNDES);                                   //
    *SPI_IER |= (1 << TXEMPTY);                                 // Enable SPI0 TX registers empty interrupt
}

void SPI0_start(void) {
    *SPI_CR |= (1 << SPIEN);                // Enable SPI0
    while(*SPI_SR & (1 << TXEMPTY));        // Wait until DMA moves first data to SPI0 TD register
    NVIC_ClearPendingIRQ(SPI0_IRQn);        // Clear pending SPI0 NVIC interrupts
    NVIC_EnableIRQ(SPI0_IRQn);              // Enable SPI0 NVIC interrupt
    gpio_set_pin_level(SPI0_DRDY, true);    // Set data ready
}
