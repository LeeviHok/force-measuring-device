all: main adc gui

main: main.c | output
	gcc main.c -std=c99 -o output/main

adc: adc.c | output
	gcc adc.c -std=c99 -D_POSIX_C_SOURCE=199309L -lwiringPi -o output/adc

gui: gui.c | output
	gcc gui.c -std=c99 `pkg-config --cflags --libs gtk+-3.0` -o output/gui

output:
	@echo "Folder $(directory) does not exist"
	mkdir -p output