#include <stdint.h>
/*
“Add a wheel speed sensor to the system. Write a function readWheelSpeed() that 
 samples an ADC (returns 0-1023 for 0-50 m/s) every 1ms, applies a 3-sample moving 
 average, and returns speed in m/s as a float.
*/
#include "filters.h"
#include "sensors.h"

float WhlSpeedBuffer[MOVING_AVG_WINDOW_SIZE_WHL_SPD] = {0};

float SteerAngBuffer[MOVING_AVG_WINDOW_SIZE_WHL_SPD] = {0};

MOV_AVG_FILT_STATE_TYPE WhlSpeedFiltState = {
                                            WhlSpeedBuffer,
                                            MOVING_AVG_WINDOW_SIZE_WHL_SPD,
                                            0,
                                            0.0f
                                            };

MOV_AVG_FILT_STATE_TYPE SteerAngFiltState = {
                                            SteerAngBuffer,
                                            MOVING_AVG_WINDOW_SIZE_STEER_ANG,
                                            0,
                                            0.0f
                                            };   

float readWheelSpeed(uint16_t adcValue) 
{
    float currWhlSpd = (float)adcValue * WHEEL_SPD_MAX/WHEEL_SPD_ADC_MAX;

    return currWhlSpd;
}