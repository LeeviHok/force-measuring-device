/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAME70 has 4 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3

#define ADC_DOUT GPIO(GPIO_PORTA, 19)
#define SPI0_SS GPIO(GPIO_PORTB, 2)
#define ADC_SCK GPIO(GPIO_PORTC, 31)
#define SPI0_MISO GPIO(GPIO_PORTD, 20)
#define SPI0_MOSI GPIO(GPIO_PORTD, 21)
#define SPI0_SCK GPIO(GPIO_PORTD, 22)
#define SPI0_DRDY GPIO(GPIO_PORTD, 25)

#endif // ATMEL_START_PINS_H_INCLUDED
