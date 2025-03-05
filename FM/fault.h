// fault.h
#ifndef FAULT_H
#define FAULT_H

#include <stdbool.h>

/* Actuator Faults */
#define ACT_CUR_MAX 819  // 4A (4/5 * 1023)
#define ACT_CUR_BUF_LEN 3 // A

/* *** */

bool detectFault(float* samples, int sampleCount, float minRange, float maxRange);

typedef struct {
                uint16_t faultBuffer[ACT_CUR_BUF_LEN];
                uint8_t fault_count;
                bool faulted;
               } FaultState;

bool detectActuatorFault(FaultState* fault, uint16_t adcCurrent);

extern FaultState actuatorFaultState;

#endif