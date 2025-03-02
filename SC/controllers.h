
#include <stdbool.h>

typedef struct {
    float KP;
    float KI;
    float KD;
    float DT;
    float INTEGRAL_MAX;
    float OUTPUT_MAX;
    float OUTPUT_MIN;
} PIDParams;

float pidControl(float setpoint, float currentSpeedFilt, bool reset, const PIDParams* params); 