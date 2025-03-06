#ifndef SPI_H

#define SPI_H
/*
Problem: SPI Transmit for Actuator Command
“Write a C function spiTransmit to send an 8-bit command to an actuator over SPI. 
Simulate a shift register and chip select (CS) toggle.”

Specs:
Inputs: uint8_t data (command, e.g., duty cycle).
Output: None (simulates SPI hardware).
Assume: 1MHz clock, MSB-first, CS low to active.
Use bit-banging (no hardware SPI for simplicity).

*/
#include <stdint.h>

#define SPI_CS_PIN 0
#define SPI_CLK_PIN 1
#define SPI_MOSI_PIN 2

void spiTransmit(uint8_t data);

#endif // SPI_H