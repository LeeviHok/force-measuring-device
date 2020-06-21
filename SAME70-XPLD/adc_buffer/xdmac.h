#include <stdint.h> // For fixed size data types

/* Register bit names */
#define DMA_CH_0    0
#define TYPE        0
#define MBSIZE      1
#define DSYNC       4
#define SWREQ       6
#define CSIZE       8
#define DWIDTH      11
#define SIF         13
#define DIF         14
#define SAM         16
#define DAM         18
#define PERID       24
#define BIE         0
#define IE0         0
#define EN0         0
#define DI0         0
#define INITD       21

/* Base memory address */
#define XDMAC_BASE_ADDR 0x40078000

/* Register pointers */
#define XDMAC_GIE       ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x0C) )   // XDMAC Global Interrupt Enable Register
#define XDMAC_GE        ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x1C) )   // XDMAC Global Channel Enable Register
#define XDMAC_GD        ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x20) )   // XDMAC Global Channel Disable Register
#define XDMAC_GS        ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x24) )   // XDMAC Global Channel Status Register
#define XDMAC_CIE0      ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x50) )   // XDMAC Channel 0 Interrupt Enable Register
#define XDMAC_CIS0      ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x5C) )   // XDMAC Channel 0 Interrupt Status Register
#define XDMAC_CSA0      ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x60) )   // XDMAC Channel 0 Source Address Register
#define XDMAC_CDA0      ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x64) )   // XDMAC Channel 0 Destination Address Register
#define XDMAC_CNDC0     ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x6C) )   // XDMAC Channel 0 Next Descriptor Control Register
#define XDMAC_CUBC0     ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x70) )   // XDMAC Channel 0 Microblock Control Register
#define XDMAC_CBC0      ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x74) )   // XDMAC Channel 0 Block Control Register
#define XDMAC_CC0       ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x78) )   // XDMAC Channel 0 Configuration Register
#define XDMAC_CDS_MSP0  ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x7C) )   // XDMAC Channel 0 Data Stride Memory Set Pattern Register
#define XDMAC_CSUS0     ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x80) )   // XDMAC Channel 0 Source Microblock Stride Register
#define XDMAC_CDUS0     ( (volatile uint32_t *)(XDMAC_BASE_ADDR + 0x84) )   // XDMAC Channel 0 Destination Microblock Stride Register


void XDMAC_init(volatile uint32_t *dest_addr);
void XDMAC_start(uint32_t data_count, int32_t *src_addr);
