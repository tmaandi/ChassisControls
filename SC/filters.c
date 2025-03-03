
#define MOVING_AVG_WINDOW 3

float movingAverage(float newSpeed) {
    static float buffer[MOVING_AVG_WINDOW] = {0};
    static int initialized_samples = 0;
    static float sum = 0;

    if (initialized_samples == MOVING_AVG_WINDOW) 
    {
        sum -= buffer[0];
    }
    else 
    {
        initialized_samples++;
    }

    for (int i = 0; i < MOVING_AVG_WINDOW - 1; i++) 
    {
        buffer[i] = buffer[i + 1];
    }

    buffer[MOVING_AVG_WINDOW - 1] = newSpeed;

    sum += newSpeed;
    
    return sum / initialized_samples;
}

float movingAverageSteering(float newAngle) {

    static float buffer[MOVING_AVG_WINDOW] = {0};
    static int initialized_samples = 0;
    static float sum = 0;

    if (initialized_samples == MOVING_AVG_WINDOW) 
    {
        sum -= buffer[0];
    }
    else 
    {
        initialized_samples++;
    }

    for (int i = 0; i < MOVING_AVG_WINDOW - 1; i++) 
    {
        buffer[i] = buffer[i + 1];
    }

    buffer[MOVING_AVG_WINDOW - 1] = newAngle;

    sum += newAngle;
    
    return sum / initialized_samples;
}