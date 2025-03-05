#ifndef FILTERS_H

#define FILTERS_H

typedef struct {
    float* buffer;
    int window_size;
    int initialized_samples;
    float sum;
   } MOV_AVG_FILT_STATE_TYPE;

float movingAverage(MOV_AVG_FILT_STATE_TYPE* state, float newSpeed); 

#endif /* FILTERS_H */

