#ifndef SENSOR_H

#define SENSOR_H

#define SPEED_THRESHOLD 5.0f  // m/s

#define WHEEL_SPD_ADC_MAX 1023

#define WHEEL_SPD_MAX 50

#define MOVING_AVG_WINDOW_SIZE_WHL_SPD 3

#define MOVING_AVG_WINDOW_SIZE_STEER_ANG 3

float readWheelSpeed(uint16_t adcValue);

extern MOV_AVG_FILT_STATE_TYPE WhlSpeedFiltState;

extern MOV_AVG_FILT_STATE_TYPE SteerAngFiltState;

#endif