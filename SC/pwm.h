#ifndef PWM_H
#define PWM_H

#include <stdint.h>

#define PWM_MAX_VAL 255
#define PWM_MIN_VAL 0

// Helper macros for min/max since we want to avoid floating point math
#define PWM_MIN(a,b) ((a) < (b) ? (a) : (b))
#define PWM_MAX(a,b) ((a) > (b) ? (a) : (b))

uint8_t pidToPwm(float pidOutput);

#endif /* PWM_H */