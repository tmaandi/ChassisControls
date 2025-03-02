
#include "controllers.h"

float pidControl(float setpoint, float currentSpeedFilt, const PIDParams* params) 
{

    float KP = params -> KP;
    float KI = params -> KI;
    float KD = params -> KD;
    float DT = params -> DT;
    float INTEGRAL_MAX = params -> INTEGRAL_MAX;
    float OUTPUT_MAX = params -> OUTPUT_MAX;
    float OUTPUT_MIN = params -> OUTPUT_MIN;

    static float integral = 0;
    static float prevError = 0;
    static int output_saturated = 0;

    float error = setpoint - currentSpeedFilt;
    float derivative = (error - prevError) / DT;
    float output = KP * error + KI * integral + KD * derivative;

    if (output > OUTPUT_MAX) {
        output = OUTPUT_MAX;
        output_saturated = 1;
    } else if (output < OUTPUT_MIN) {
        output = OUTPUT_MIN;
        output_saturated = -1;
    } else {
        output_saturated = 0;
    }

    if (output_saturated * error <= 0) {
        integral += error * DT;
        if (integral > INTEGRAL_MAX) integral = INTEGRAL_MAX;
        else if (integral < -INTEGRAL_MAX) integral = -INTEGRAL_MAX;
    }

    prevError = error;
    return output;
}