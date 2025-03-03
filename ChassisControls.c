#include <stdint.h>

#ifndef PC_BUILD
#include "pico/stdlib.h"
#endif

#include "abs.h"
#include "filters.h"

const PIDParams params = {  1.00F, /* KP */  
                            0.10F, /* KI */  
                            0.05F, /* KD */ 
                            0.01F, /* DT */ 
                           50.00F, /* INTEGRAL_MAX */ 
                          100.00F, /* OUTPUT_MAX */
                          -100.0F  /* OUTPUT_MIN */ 
                         };

int main() {

    float speeds[] = {10, 8, 6, 6, 8, 10, 14, 16, 14};

    int num_speeds = sizeof(speeds)/sizeof(float);

    float setpoint = 10.0F;

    AbsState abs_state = IDLE;

    uint8_t controlCmd = 0;

    float filtered_whl_spd;
    const char* absStateStr;

    for (int i = 0; i < num_speeds; i++) 
    {

        filtered_whl_spd = movingAverage(speeds[i]);

        abs_state = absControl(setpoint, filtered_whl_spd, &controlCmd, &params);

        switch (abs_state) {
            case IDLE:
            absStateStr = "IDLE";
            break;
            case APPLY:
            absStateStr = "APPLY";
            break;
            case RELEASE:
            absStateStr = "RELEASE";
            break;
            default:
            absStateStr = "UNKNOWN";
            break;
        }
        
        printf("Setpoint: %.2f, Filtered Speed: %.2f, AbsState: %s, Control: %d\n", setpoint, filtered_whl_spd, absStateStr, controlCmd);
    }
    
    return 0;
}
