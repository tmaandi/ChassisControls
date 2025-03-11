/*

Problem: CAN Message Transmission
“Write a C function canTransmit to send an 8-byte CAN data frame with a given 11-bit identifier. Simulate the frame construction and transmission, focusing on bitwise operations for header and data packing.”

Specs:
Inputs:
uint16_t id (11-bit identifier, 0x000-0x7FF).
uint8_t* data (8-byte payload, e.g., actuator state or sensor readings).
Output: None (prints simulated frame—ID, data, CRC placeholder).
Format: Standard CAN 2.0A (11-bit ID):
Start of Frame (SOF): 0.
11-bit Identifier.
RTR (Remote Transmission Request): 0 (data frame).
4-bit DLC (Data Length Code): 8 (fixed 8 bytes).
8-byte Data.
CRC (15-bit, placeholder here).
ACK slot (1, placeholder).
End of Frame (EOF): 7 bits of 1.
Constraints: Embedded-friendly—no dynamic allocation, bitwise focus.

*/

#include <stdio.h>
#include "can.h"

void canTransmit(uint16_t id, uint8_t* data) {
    if (id > CAN_ID_MAX || data == NULL) {
        printf("CAN Error: Invalid ID or data\n");
        return;
    }

    printf("SOF: 0\n");

    printf("ID: ");
    for (int i = 10; i >= 0; i--) {
        printf("%d", (id >> i) & 1);
    }
    printf("\n");

    printf("RTR: 0\n");

    printf("DLC: ");
    for (int i = 3; i >= 0; i--) {
        printf("%d", (CAN_DLC >> i) & 1);  // 8 = 0b1000
    }
    printf("\n");

    printf("Data: ");
    for (int i = 0; i < CAN_DLC; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (data[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");

    printf("CRC: [15-bit placeholder]\n");
    printf("ACK: 1\n");
    printf("EOF: 1111111\n");
}

/*
Task: “Write a C function parseCanMessage to extract an 11-bit ID and 8-byte data from 
a CAN 2.0 A frame buffer (simplified). Print ID and first 2 data bytes.”

Inputs: uint8_t* buffer (assume 11 bytes: ID high byte, ID low byte, 8 data bytes).
Output: None (prints parsed values).
Constraints: Bitwise ops, no heap.
Example:

Buffer: {0x01, 0x23, 0xAA, 0xBB, 0, 0, 0, 0, 0, 0} → ID = 0x123, data[0] = 0xAA, data[1] = 0xBB.
Start: “Let’s break it down”—solve it! Code here, think aloud.
*/
void parseCanMessage(uint8_t* buffer)
{
    if (buffer == NULL)
    {
        printf("Error: Null buffer\n");
        return;
    }

    uint16_t id = 0;

    id = ((uint16_t)buffer[0] << 8) | buffer[1];

    if (id > 0x7FF)
    {
        printf("Error: ID 0x%04X exceeds 11-bit max for CAN 2.0A\n",id);
        return;
    }

    printf("CAN ID: 0x%03X\n",id);

    printf("Byte 0: %02X ",buffer[2]);

    printf("Byte 1: %02X \n",buffer[3]);
}