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
#include "fault.h"

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