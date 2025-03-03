/*
Problem
“Write a C function pidToPwm that converts the PID output from pidControl (range: -100.0 to 100.0) 
into a PWM duty cycle (range: 0 to 255) for an actuator. The PWM signal should control a brake or 
steering actuator where:

-100.0 → full reverse/off (0),
0.0 → neutral/midpoint (127.5 ≈ 128),
100.0 → full forward/max (255).”
Specs
Input: float pidOutput (from pidControl, -100.0 to 100.0).
Output: uint8_t (0 to 255)—8-bit PWM duty cycle.
Constraints:
Linear mapping.
No floating-point math in final output (integer return).
Embedded-friendly—minimal overhead.
*/
#include <stdint.h>  // For uint8_t
#include "pwm.h"

uint8_t pidToPwm(float pidOutput) {
    float scaled = (pidOutput + 100.0f) * (255.0f / 200.0f); // Exact scaling

    uint8_t pwm = (uint8_t)(scaled + 0.5f); // Round up
    
    return PWM_MIN(PWM_MAX(PWM_MIN_VAL, pwm), PWM_MAX_VAL);
}
