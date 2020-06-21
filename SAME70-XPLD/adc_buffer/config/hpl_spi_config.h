/* Auto-generated config file hpl_spi_config.h */
#ifndef HPL_SPI_CONFIG_H
#define HPL_SPI_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

#include <peripheral_clk_config.h>

// Enable configuration of module
#ifndef CONF_SPI_0_ENABLE
#define CONF_SPI_0_ENABLE 1
#endif

// Set module in SPI Slave mode
#ifndef CONF_SPI_0_MODE
#define CONF_SPI_0_MODE 0x0
#endif

// Set FIFO disable
#ifndef CONF_SPI_0_FIFO_DISABLE
#define CONF_SPI_0_FIFO_DISABLE 0x01
#endif

#define CONF_SPI_0_PS 0x0
#define CONF_SPI_0_PCSDEC 0x0
#define CONF_SPI_0_BAUD_RATE 0x0
#define CONF_SPI_0_DLYBS 0x0
#define CONF_SPI_0_DLYBCT 0x0
#define CONF_SPI_0_DUMMYBYTE 0x0

// <h> Basic Configuration

// <o> Character Size
// <i> Bit size for all characters sent over the SPI bus (CHSIZE)
// <0x0=>8 bits
// <0x1=>9 bits
// <0x2=>10 bits
// <0x3=>11 bits
// <0x4=>12 bits
// <0x5=>13 bits
// <0x6=>14 bits
// <0x7=>15 bits
// <0x8=>16 bits
// <id> spi_slave_character_size
#ifndef CONF_SPI_0_CHSIZE
#define CONF_SPI_0_CHSIZE 0x0
#endif

// </h>

// <e> Advanced Configuration
// <id> spi_slave_advanced
#ifndef CONF_SPI_0_ADVANCED
#define CONF_SPI_0_ADVANCED 1
#endif

// <o> Clock Polarity
// <0=>The inactive state value of SPCK is logic level zero.
// <1=>The inactive state value of SPCK is logic level one.
// <i> Determines the inactive state value of the serial clock (SPCK).
// <id> spi_slave_arch_cpol
#ifndef CONF_SPI_0_CPOL
#define CONF_SPI_0_CPOL 0x0
#endif

// <o> Clock Phase
// <0x0=>Data is changed on the leading edge of SPCK and captured on the following edge of SPCK.
// <0x1=>Data is captured on the leading edge of SPCK and changed on the following edge of SPCK.
// <i> Determines which edge of SPCK causes data to change and which edge causes data to be captured.
// <id> spi_slave_arch_cpha
#ifndef CONF_SPI_0_NCPHA
#define CONF_SPI_0_NCPHA 0x1
#endif

// </e>

// <<< end of configuration section >>>

#endif // HPL_SPI_CONFIG_H
