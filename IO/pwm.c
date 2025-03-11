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
#include "fault.h"
#include "pwm.h"

uint8_t pidToPwm(float pidOutput) {
    float scaled = (pidOutput + 100.0f) * (255.0f / 200.0f); // Exact scaling

    uint8_t pwm = (uint8_t)(scaled + 0.5f); // Round up
    
    return PWM_MIN(PWM_MAX(PWM_MIN_VAL, pwm), PWM_MAX_VAL);
}

/*
Problem: “Write a C function pwmActuator(uint8_t dutyCycle) that outputs a PWM signal (0 or 1)
 for a brake solenoid. It’s called every 1ms in an interrupt, with a 100ms period. dutyCycle is 0-100%.”

 Next: Then, adjust pwmActuator to reduce duty cycle if speed drops below 10 m/s (brake less if slipping).”
*/

uint8_t pwmActuator(uint8_t dutyCycle, FaultState* faultState) {

    static uint8_t tick_count = 0;

    uint8_t output = 0; // Explicit init

    // Clamp duty cycle to 0-100%
    if (dutyCycle > 100) {
        dutyCycle = 100;
    }

    // Increment and reset first
    tick_count++;

    if (tick_count >= PWM_PERIOD_MS) {
        tick_count = 0;
    }

    if (faultState->faulted == true)
    {
        return 0;
    }
    // Set output based on duty cycle
    if (tick_count <= dutyCycle) {
        output = 1;
    }

    return output;
}

/*
PWM Duty Limiter
Task: “Write a C function limitPwmDuty to rescale a PWM duty cycle (0-255) between a min (50) and max (200). 
Use a 1D lookup table for scaling if input exceeds bounds.”

Inputs: uint8_t duty (raw input).
Output: uint8_t (scaled duty).
Constraints: No floats, embedded-friendly.
Example:

duty = 30 → 50 (min).
duty = 150 → 150 (within).
duty = 210 → 200 (max).
Start: “Let’s break it down”—solve it! Code here, think aloud.
*/

const uint8_t PWM_SCALE_LUT[2] = {0, 255};

#define PWM_LB (uint8_t)50
#define PWM_UB (uint8_t)200

uint8_t limitPwmDuty(uint8_t input)
{
    uint8_t scaled_input = 0;

    scaled_input = PWM_LB + (uint8_t)(((uint16_t)(input - PWM_SCALE_LUT[0])*(PWM_UB - PWM_LB))/(PWM_SCALE_LUT[1] - PWM_SCALE_LUT[0]));

    return scaled_input;
}