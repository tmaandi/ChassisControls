
#include "filters.h"

float movingAverage(MOV_AVG_FILT_STATE_TYPE* state, float input) 
{
    if (state -> initialized_samples == state -> window_size) 
    {
        state -> sum -= state -> buffer[0];
    }
    else 
    {
        state -> initialized_samples++;
    }

    for (int i = 0; i < state -> window_size - 1; i++) 
    {
        state -> buffer[i] = state -> buffer[i + 1];
    }

    state -> buffer[state->window_size - 1] = input;

    state -> sum += input;
    
    return (state -> sum) / (state -> initialized_samples);
}

void initLowPassFilter(LOW_PASS_FILT_STATE_TYPE* state) {
    state->prevOutput = 0;  // Initial value
}

uint16_t lowPassFilter(LOW_PASS_FILT_STATE_TYPE* state, uint16_t cutOffFreq, uint16_t sampleFreq, uint16_t input) {
    // Alpha = (2π f_c T_s) / (1 + 2π f_c T_s), T_s = 1/sampleFreq
    uint32_t Ts = 1000 / sampleFreq;  // ms, e.g., 1000 Hz = 1 ms
    uint32_t temp = 2 * 314 * cutOffFreq * Ts;  // 2π f_c T_s * 100
    uint8_t alpha = (uint8_t)(temp * ALPHA_SCALE/ (100 + temp));   // Scale first, then divide
    // y[n] = αx[n] + (1-α)y[n-1], integer math
    uint32_t filtVal = (alpha * input + (ALPHA_SCALE - alpha) * state->prevOutput) / ALPHA_SCALE;
    state->prevOutput = (uint16_t)filtVal;  // Update state
    return state->prevOutput;
}