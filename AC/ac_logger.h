#ifndef AC_LOGGER_H_
#define AC_LOGGER_H_

#include <stdint.h>

typedef struct
{
    uint16_t current;

    uint8_t pwmDuty;

    uint8_t faultFlag;
    
} AC_LOGGER_TYPE;

void logBrakeActuatorState(AC_LOGGER_TYPE* brakeAcState);

#endif