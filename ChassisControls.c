#include <stdio.h>

#ifndef PC_BUILD
#include "pico/stdlib.h"
#endif

#include "filters.h"
#include "controllers.h"

const PIDParams params = {  1.00F, /* KP */  
                            0.10F, /* KI */  
                            0.05F, /* KD */ 
                            0.01F, /* DT */ 
                           50.00F, /* INTEGRAL_MAX */ 
                          100.00F, /* OUTPUT_MAX */
                          -100.0F  /* OUTPUT_MIN */ 
                         };

int main() {
    float speeds[] = {10, 8, 6, 8, 10};
    float setpoint = 10.0F;
    for (int i = 0; i < 5; i++) {
        float filtered = movingAverage(speeds[i]);
        float control = pidControl(setpoint, filtered, &params);
        printf("Speed: %.2f, Filtered: %.2f, Control: %.2f\n", speeds[i], filtered, control);
    }
    return 0;
}
