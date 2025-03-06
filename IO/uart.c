/*
Problem: UART Transmit for Fault Logging
“Write a C function uartTransmit to send a byte over UART, simulating bit-by-bit transmission.”

Specs:
Input: uint8_t data (e.g., 0x41 for ‘A’).
Output: None (prints bits to simulate TX pin).
Format: 9600 baud, 8N1 (start bit, 8 data bits, no parity, 1 stop bit).
Baud Rate: 9600 bps → ~104µs per bit (we’ll simulate with printf).
*/
#include <stdio.h>

#include "uart.h"

void uartTransmit(uint8_t data)
{
    // Drop Tx Low - Start of Transmission
    // gpio_put(UART_TX_PIN, 0);  // Start bit
    // sleep_us(104);             // 9600 baud = 104µs
    printf("UART TX PIN LOW %d\n",0);

    for (int i = 0; i < 8; i++)
    {
        // gpio_put(UART_TX_PIN, (data >> i) & 1);
        // sleep_us(104);
        printf("%d",(data >> i) & 1);
    }

    // STOP Bit
    printf("\nSTOP BIT %d\n",1);

    // Set Tx PIN High, Stop Transmission
    printf("UART TX PIN HIGH %d\n",1);

}