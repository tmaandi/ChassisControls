#ifndef I2C_H
#define I2C_H

#include <stdint.h>

uint16_t readBrakePressureSensor(uint8_t sensorAddr, uint8_t pressureReg, uint8_t* pressureData); 

#endif