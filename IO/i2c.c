/*
I2C Brake Pressure Sensor Read
Task: “Write a C function readBrakePressureSensor to read 2 bytes of pressure data (0-5000 kPa) 
from an I2C sensor at address 0x1A, register 0x05 (pressure reg). Send address + write to select
the register, then address + read to get data. Simulate with printf—fake data is fine.”

Inputs: None (hardcode addr 0x1A, reg 0x05; simulate 2-byte data, e.g., 0x0A 0xF0 = 2800 kPa).
Output: uint16_t (pressure in kPa).
Constraints: Bitwise ops, no heap, embedded-friendly.
Example:

Send: “START, Address: 0x34 (write), Reg: 0x05, STOP”.
Send: “START, Address: 0x35 (read), STOP”.
Receive: “Data: 0x0A 0xF0” → 2800 kPa (0x0AF0).
Context: This mimics reading a brake pressure sensor (e.g., BMP280-style) for your ABS system—real 
integration task.
*/

#include <stdio.h>
#include "i2c.h"

uint16_t readBrakePressureSensor(uint8_t sensorAddr, uint8_t pressureReg, uint8_t* pressureData) 
{
    // Write: Select register
    uint8_t writeAddr = (sensorAddr << 1);  // 0x1A -> 0x34 (R/W = 0)
    printf("START, Address: 0x%02X (write), Reg: 0x%02X, STOP\n", writeAddr, pressureReg);

    // Read: Get 2 bytes
    uint8_t readAddr = (sensorAddr << 1) | 1;  // 0x1A -> 0x35 (R/W = 1)
    printf("START, Address: 0x%02X (read), STOP\n", readAddr);
    uint16_t pressure = (pressureData[0] << 8) | pressureData[1];  // 0x0A, 0xF0 -> 0x0AF0
    printf("Data: 0x%02X 0x%02X (Pressure: %u kPa)\n", pressureData[0], pressureData[1], pressure);

    return pressure;
}