/*
Fault Detection Challenge
Problem
“Write a function detectFault to identify stuck or out-of-range wheel speed or steering angle sensors.”

Inputs:
float* samples: Array of recent values (e.g., last 5).
int sampleCount: Number of samples.
float minRange, float maxRange: Valid range (e.g., 0-100 m/s for speed, ±50° for angle).
Output: bool—true if faulty (stuck: no change; out-of-range: beyond limits).
Constraints: Embedded-friendly—no heavy math or dynamic allocation.
*/

// fault.c
#include <stdint.h>
#include "fault.h"

FaultState actuatorFaultState = {
                                {0, 0, 0},
                                0,
                                true                                    
                                };

bool detectFault(float* samples, int sampleCount, float minRange, float maxRange) 
{
    // Check stuck (all equal) or out-of-range

    bool fault_flag = false;

    bool stuck_flag = true;

    float stuck_val = samples[0];

    for (int i = 0; i < sampleCount; i++)
    {
        /* Range Check */
        if ((samples[i] > maxRange) || (samples[i] < minRange))
        {
            fault_flag = true;
            break;
        }

        if (samples[i] != stuck_val)
        {
            stuck_flag = false;
        }

    }

    if (stuck_flag == true)
    {
        fault_flag = true;
    }

    return fault_flag;
}

/*
“Write a C function detectActuatorFault that monitors an actuator’s current draw 
(ADC value, 0-1023 for 0-5A) and shuts it down if it exceeds a threshold (e.g., 4A)
for 3 consecutive samples. Use a state struct to track history. Update pwmActuator
to respect the fault state.”
*/

bool detectActuatorFault(FaultState* state, uint16_t adcCurrent)
{
     /* Update Fault Buffer via left shift
        Need fault buffer to store extended fault record, no 
        need of it for fault logic
     */
    for (int i = 0; i < ACT_CUR_BUF_LEN-1; i++)
    {
        state->faultBuffer[i] = state->faultBuffer[i+1];
    }

    state->faultBuffer[ACT_CUR_BUF_LEN-1] = adcCurrent;


    if (adcCurrent > ACT_CUR_MAX)
    {
        if (state->fault_count < ACT_CUR_BUF_LEN)
        {
            state->fault_count++;
        }
    }
    else
    {
        state->fault_count = 0;
        state->faulted = false;
    }

    if (state->fault_count == ACT_CUR_BUF_LEN)
    {
        state->faulted = true;
    }
    else
    {
        state->faulted = false;
    }

    return (state->faulted);
}