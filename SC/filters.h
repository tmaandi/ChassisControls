#ifndef FILTERS_H
#define FILTERS_H

#include <stdint.h>

typedef struct {
    float* buffer;
    int window_size;
    int initialized_samples;
    float sum;
   } MOV_AVG_FILT_STATE_TYPE;

typedef struct {
                uint16_t prevOutput;
               } LOW_PASS_FILT_STATE_TYPE;

#define ALPHA_SCALE 100  // Scale factor for fixed-point

float movingAverage(MOV_AVG_FILT_STATE_TYPE* state, float input); 

void initLowPassFilter(LOW_PASS_FILT_STATE_TYPE* state);

uint16_t lowPassFilter(LOW_PASS_FILT_STATE_TYPE* state, uint16_t cutOffFreq, uint16_t sampleFreq, uint16_t input); 

#endif /* FILTERS_H */

