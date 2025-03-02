
#define KP 1.0F
#define KI 0.1F
#define KD 0.05F
#define DT 0.01F
#define INTEGRAL_MAX 50.0F
#define OUTPUT_MAX 100.0F
#define OUTPUT_MIN -100.0F

float pidControl(float setpoint, float currentSpeedFilt) {
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