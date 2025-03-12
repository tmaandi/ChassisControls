/*
Write a C function logBrakeActuatorState to log a brake actuator’s state over CAN. 
Input is a struct with PWM duty (0-255), current (0-5000 mA),
and fault flag. Pack into an 8-byte CAN frame (ID 0x124) and simulate with printf
*/

#include <stdio.h>

#include "ac_logger.h"

void logBrakeActuatorState(AC_LOGGER_TYPE* brakeAcState)
{
    if (brakeAcState == NULL) {
        printf("Error: Null state\n");
        return;
    }
    
    uint16_t canID = 0x124;

    uint8_t canDLC = 8;

    uint8_t canData[8] = {0};

    canData[0] = brakeAcState->pwmDuty;

    canData[1] = (uint8_t)(brakeAcState->current >> 8);

    canData[2] = (uint8_t)(brakeAcState->current);

    canData[3] = brakeAcState->faultFlag;

    printf("CAN ID: 0x%03X, ",canID);

    printf("CAN Data: ");

    for (int i = 0; i < canDLC; i++)
    {
        printf(" %02X ",canData[i]);
    }
}