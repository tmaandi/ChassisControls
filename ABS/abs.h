#include "controllers.h"

typedef enum { IDLE,   // 0
               APPLY,  // 1
               RELEASE // 2
             } AbsState;

typedef enum { SPD_ON_TARGET,   // 0
               SPD_OVER_TARGET, // 1
               SPD_UNDER_TARGET,// 2
            } AbsEvent;

AbsState absControl(float setpoint, float currentSpeedFilt, uint8_t* controlCmd, const PIDParams* params);   

/* ABS Parameters */
#define ABS_WHL_SPEED_HYST 1.0f