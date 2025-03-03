#ifndef STEER_H
#define STEER_H

#include <stdint.h>
#include <controllers.h>

typedef enum {LEFT,
              CENTER,
              RIGHT
             } SteeringState;

typedef enum {ANGLE_ON_TARGET,
              ANGLE_LEFT,
              ANGLE_RIGHT
             } SteeringEvent;

#define STEERING_ANG_HYST 2 // deg?

SteeringState steeringControl(float setpoint, float currentAngleFilt, uint8_t* controlCmd, const PIDParams* params);

#endif /* STEER_H */