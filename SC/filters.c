
#include "filters.h"

float movingAverage(MOV_AVG_FILT_STATE_TYPE* state, float newSpeed) 
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

    state -> buffer[state->window_size - 1] = newSpeed;

    state -> sum += newSpeed;
    
    return (state -> sum) / (state -> initialized_samples);
}