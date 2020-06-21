#include <atmel_start.h>
#include <stdint.h>     // For fixed size data types
#include <stdio.h>      // For debugging
#include "xdmac.h"


void XDMAC_init(volatile uint32_t *dest_addr) {
    if(*XDMAC_GS & (1 << DMA_CH_0)) {                   // If DMA channel 0 is already in use
        printf("ERROR: DMA channel already in use.");   // Print error message
        while(1);                                       // Get stuck here
    }
    *XDMAC_CIS0;                            // Clear pending interrupts by reading register
    *XDMAC_CDA0 = (uintptr_t)dest_addr;     // Set DMA destination address
    *XDMAC_CUBC0 = 1;                       // Set microblock size to 1 data
    *XDMAC_CC0 |= (1 << TYPE);              // Set type to peripheral type
    *XDMAC_CC0 &= ~(3 << MBSIZE);           // Set burst size to 1
    *XDMAC_CC0 &= ~(3 << SAM);              // Set Source Addressing Mode to incremented
    *XDMAC_CC0 |= (1 << SAM);               //
    *XDMAC_CC0 &= ~(3 << DAM);              // Set Destination Addressing Mode to unchanged
    *XDMAC_CC0 |= (1 << DSYNC);             // Set Memory-to-Peripheral transfer mode
    *XDMAC_CC0 &= ~(7 << CSIZE);            // Set Channel 0 chunk size to 1 data
    *XDMAC_CC0 &= ~(3 << DWIDTH);           // Set Channel 0 Data Width to 8 bits
    *XDMAC_CC0 &= ~(1 << SIF);              // Set Channel 0 read to interface 0
    *XDMAC_CC0 |= (1 << DIF);               // Set Channel 0 write to interface 1
    *XDMAC_CC0 &= ~(127 << PERID);          // Set Channel 0 Peripheral Hardware Request Line Identifier to SPI0 Transmit
    *XDMAC_CC0 |= (1 << PERID);             //
    *XDMAC_CC0 &= ~(1 << SWREQ);            // Set Channel 0 to use hardware request line
    *XDMAC_CNDC0 = 0;                       // Clear XDMAC_CNDC0
    *XDMAC_CDS_MSP0 = 0;                    // Clear XDMAC_CDS_MSP0
    *XDMAC_CSUS0 = 0;                       // Clear XDMAC_CSUS
    *XDMAC_CDUS0 = 0;                       // Clear XDMAC_CDUS0
    *XDMAC_CIE0 |= (1 << BIE);              // Enable XDMAC channel 0 block interrupt
    *XDMAC_GIE |= (1 << IE0);               // Enable XDMAC channel 0 global interrupt
}

void XDMAC_start(uint32_t data_count, int32_t *src_addr) {
    *XDMAC_CSA0 = (intptr_t)src_addr;       // Set DMA source address
    *XDMAC_CBC0 = data_count * 4 - 1;       // Set block length
    *XDMAC_GE |= (1 << EN0);                // Enable XDMAC channel 0
    while(!(*XDMAC_CC0 & (1 << INITD)));    // Wait until channel 0 init is done
}
