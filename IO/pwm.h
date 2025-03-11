#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "fault.h"

#define PWM_MAX_VAL 255
#define PWM_MIN_VAL 0

#define PWM_PERIOD_MS 100

// Helper macros for min/max since we want to avoid floating point math
#define PWM_MIN(a,b) ((a) < (b) ? (a) : (b))
#define PWM_MAX(a,b) ((a) > (b) ? (a) : (b))

uint8_t pidToPwm(float pidOutput);

uint8_t pwmActuator(uint8_t dutyCycle, FaultState* faultState);

uint8_t limitPwmDuty(uint8_t input);

#endif /* PWM_H */