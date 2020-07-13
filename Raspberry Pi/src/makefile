output_dir = ../output

all: main adc gui websocket

main: main.c | $(output_dir)
    gcc main.c -std=c99 -o $(output_dir)/main

adc: adc.c | $(output_dir)
    gcc adc.c -std=c99 -D_POSIX_C_SOURCE=199309L -lwiringPi -o $(output_dir)/adc

gui: gui.c | $(output_dir)
    gcc gui.c -std=c99 `pkg-config --cflags --libs gtk+-3.0` -o $(output_dir)/gui

websocket: websocket.c | $(output_dir)
    gcc websocket.c -std=c99 -o $(output_dir)/websocket

$(output_dir):
    mkdir -p $(output_dir)
