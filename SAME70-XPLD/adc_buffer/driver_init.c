/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_spi_base.h>

struct spi_s_async_descriptor SPI_0;
static uint16_t               SPI_0_buf[16];

void SPI_0_PORT_init(void)
{

	gpio_set_pin_function(SPI0_MISO, MUX_PD20B_SPI0_MISO);

	gpio_set_pin_function(SPI0_MOSI, MUX_PD21B_SPI0_MOSI);

	gpio_set_pin_function(SPI0_SS, MUX_PB2D_SPI0_NPCS0);

	gpio_set_pin_function(SPI0_SCK, MUX_PD22B_SPI0_SPCK);
}

void SPI_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_SPI0);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_s_async_set_func_ptr(&SPI_0, _spi_get_spi_s_async());
	spi_s_async_init(&SPI_0, SPI0, (uint8_t *)SPI_0_buf, 32);
	SPI_0_PORT_init();
}

void system_init(void)
{
	init_mcu();

	_pmc_enable_periph_clock(ID_PIOA);

	_pmc_enable_periph_clock(ID_PIOC);

	_pmc_enable_periph_clock(ID_PIOD);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PA19 */

	// Set pin direction to input
	gpio_set_pin_direction(ADC_DOUT, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(ADC_DOUT,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(ADC_DOUT, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PC31 */

	gpio_set_pin_level(ADC_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(ADC_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ADC_SCK, GPIO_PIN_FUNCTION_OFF);

	/* GPIO on PD25 */

	gpio_set_pin_level(SPI0_DRDY,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SPI0_DRDY, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI0_DRDY, GPIO_PIN_FUNCTION_OFF);

	SPI_0_init();
}
