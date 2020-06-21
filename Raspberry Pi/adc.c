#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define DRDY    0
#define RESET   1
#define MOSI    12
#define MISO    13
#define SCLK    14
#define CE0     10

#define ADC_GAIN                128
#define ADC_MAX_COUNT           8388608
#define SENSOR_RATED_FORCE      100
#define SENSOR_SENSITIVITY      0.00200064
#define NEWTON_PER_ADC_COUNT    (SENSOR_RATED_FORCE / (ADC_GAIN * SENSOR_SENSITIVITY * ADC_MAX_COUNT))

void sigterm();
void DRDY_handler(void);
int32_t read_adc(void);

int read_pipe, write_pipe;


int main(int argc, char *argv[]) {
	/* Process initialization */
	signal(SIGTERM, sigterm);
	write_pipe = strtol(argv[4], NULL, 10);
	read_pipe = strtol(argv[2], NULL, 10);
	close(strtol(argv[1], NULL, 10));		// Close useless pipe ends
	close(strtol(argv[3], NULL, 10));		//

	/* Setup GPIO pins */
	wiringPiSetup();
	pinMode(DRDY, INPUT);
	pinMode(RESET, OUTPUT);
	pullUpDnControl(DRDY, PUD_DOWN);
	digitalWrite(RESET, LOW);

	/* Wait 200 us for ADC circuitry to reset */
	struct timespec req, rem;
	req.tv_sec = 0;
	req.tv_nsec = 200 * 1000;
	nanosleep(&req , &rem);

	/* Setup Interrupts */
	wiringPiISR(DRDY, INT_EDGE_RISING, &DRDY_handler);

	/* Setup SPI */
	wiringPiSPISetup(0, 500000);

	/* Enable ADC */
	digitalWrite(RESET, HIGH);

	while(1) {
		sleep(UINT_MAX);
	}
}


void sigterm() {
	pinMode(DRDY, INPUT);
	pinMode(RESET, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, INPUT);
	pinMode(SCLK, INPUT);
	pinMode(CE0, INPUT);
	pullUpDnControl(DRDY, PUD_OFF);
	pullUpDnControl(RESET, PUD_OFF);
	pullUpDnControl(MOSI, PUD_OFF);
	pullUpDnControl(MISO, PUD_OFF);
	pullUpDnControl(SCLK, PUD_OFF);
	pullUpDnControl(CE0, PUD_OFF);

	close(read_pipe);
	close(write_pipe);

	exit(1);
}

void DRDY_handler(void) {
	static int32_t sample;
	static float force;

	sample = read_adc();
	force = sample * NEWTON_PER_ADC_COUNT;
	write(write_pipe, &force, sizeof(force));
}

int32_t read_adc(void) {
	static uint8_t buffer[4];
	static int32_t value;
	value = 0;

	/* Data arrives LSB first */
	wiringPiSPIDataRW(0, buffer, 4);
	value |= (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];
	return value;
}
