/* Register bit names */
#define WPITEN      1
#define WPCREN      2
#define WPKEY       8
#define SPIEN       0
#define RDRF        0
#define TDRE        1
#define MODF        2
#define OVRES       3
#define NSSR        8
#define TXEMPTY     9
#define UNDES       10
#define SPIDIS      1

/* Base memory address */
#define SPI0_BASE_ADDR  0x40008000

/* Register pointers */
#define SPI_CR          ( (volatile uint32_t *)(SPI0_BASE_ADDR + 0x00) )    // SPI0 Control Register
#define SPI0_TDR        ( (volatile uint32_t *)(SPI0_BASE_ADDR + 0x0C) )    // SPI0 Transmit Data Register
#define SPI_SR          ( (volatile uint32_t *)(SPI0_BASE_ADDR + 0x10) )    // SPI0 Status Register
#define SPI_IER         ( (volatile uint32_t *)(SPI0_BASE_ADDR + 0x14) )    // SPI0 Interrupt Enable Register
#define SPI_IDR         ( (volatile uint32_t *)(SPI0_BASE_ADDR + 0x18) )    // SPI0 Interrupt Disable Register
#define SPI0_WPMR       ( (volatile uint32_t *)(SPI0_BASE_ADDR + 0xE4) )    // SPI0 Write Protection Mode Register

#define PASSWD 0x535049 // SPI0 write protection password


void SPI0_init(void);
void SPI0_start(void);
