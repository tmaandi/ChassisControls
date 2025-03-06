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