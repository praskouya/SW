# SW

This repository contains projects completed as part of the "Embedded Systems" course at Łódź University of Technology. Those projects are designed for the STM32L496ZGT6 microcontroller using the KAmeleon-STM32L4 evaluation board. The IDE utilized is STM32CubeIDE.

## Contents

The repository includes the following projects:

### Project 1: LED 

A program controlling an LED and an RGB LED on based on joystick input, while simultaneously making the LED blink at a frequency of approximately 1 Hz. Joystick input corresponding: Red (R) for the left button, Green (G) for the down button, Blue (B) for the right button. When a button is pressed, the corresponding LED color should be turned on. Pressing the center joystick button should light up all RGB colors simultaneously.



### Project 2: Snake

A program creating a "snake-like" LED effect where an illuminated LED appears to move sequentially across the LEDs D1-D8, first in one direction and then in reverse. This project has an implemented timing control to ensure that the LED transition from one to the next happens smoothly and at a consistent speed. HAL 



### Project 3: Counter

A counter implementation that counts up and down within the range of 0-9 using a single digit on a 7-segment display. Each digit is updated every second, counting direction is controlled by the OK button on the joystick (When the button is not pressed - up; pressed - down). The programm also allows easy change of the segment used for displaying values.


### Project 4: Timer

This project is a simple timer implementation. It uses all 4 segments of the evaluation board to display numbers from 0 to 1000.  Each digit is updated every second, counting direction is controlled by the OK button on the joystick (When the button is not pressed - up; pressed - down). This program is written without using the HAL library.


### Project 5: UART Data Transmission

This program enables communication between a computer and the STM microcontroller via the UART interface. The program converts the case of ASCII characters received from the computer and then sends them back. The UART communication parameters are set to 8N1 (8 data bits, no parity, 1 stop bit).

### Project 6: FIFO Queue

This project implements a FIFO (First-In, First-Out) queue that buffers received ASCII characters on the microcontroller. When a space character is received, the program outputs one element from the buffer. If a period is received, the program clears the entire buffer and outputs all its contents. The buffer size is initially set to 6, but this can easily be changed by modifying the ' #define BUFFER_SIZE 5' directive.