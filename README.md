# force-measuring-device

Force measuring device is project where force sensor, ADC, real-time MCU and Raspberry Pi are used to measure and display force. This project is built into a shoebox and was intended as an exercise to become more familiar with ATSAME70 MCU. Electronics were kept minimal and only custom-made part was circuit board for ADC built by using stripboard and off-the-shelf components.

![Image from the device](Images/Force_measuring_device_w_sensor.jpg)

## Components

| Component        | Manufacturer            | Model           |
| ---------------- | ----------------------- | --------------- |
| Force sensor     | HBM                     | S2M/100N        |
| ADC              | Avia Semiconductor      | HX711           |
| Real-time MCU    | Microchip               | SAME70 Xplained |
| Raspberry Pi 3   | Raspberry Pi Foundation | Model B+        |
| LCD touch screen | Joy-IT                  | RB-LCD-7-2      |

### Force sensor
Force sensor is typical resistive wheatstone bridge sensor rated for 100 Newtons. It outputs differential signal with rated sensitivity of 2 mV/V on top of common mode voltage, which is half of the excitation voltage. It has 6-wire configuration, but sense wires cannot be used because ADC does not have external reference voltage pins.

### ADC
ADC has differential inputs and on-chip amplifier, so it can be used directly to read force sensor. Amplifier has selectable gain, with options of 32, 64 and 128. ADC features also on-board analog supply regulator, which is used with few external components to create excitation voltage for force sensor.

### Real-time MCU
SAME70 Xplained evaluation board is used as a real-time MCU to read and buffer ADC samples before transferring data to Raspberry Pi over SPI. SAME70 is based on ARM Cortex-M7 with lots of different peripherals and it runs at 300 MHz. It is huge overkill for this application, but one needs to remember that main purpose of this project was to become more familiat with this MCU. Raspberry Pi could be also used to read ADC directly, but it may lose few samples every now and then as it does not have real-time operating system.

### Raspberry Pi
Raspberry Pi is used to run graphical user interface, which displays measured force. It reads data over SPI from SAME70 before passing it to GUI. SPI and GPIO pins are accessed by using WiringPi library, which provides nice abstraction from hardware. Raspberry Pi is also hooked to LCD touch screen to show that awesome GUI to the world.

### LCD touch screen
This device has built-in 7" LCD touch screen with resolution of 1024 x 600 pixels. This way external keyboard and mouse are not necessary, altough bit more convinient. It makes this also truly embedded system, which does not need any external devices to operate.

## Internal view from the device

![Image from inside of the device](Images/Force_measuring_device_inside_w_sensor.jpg)
