
#include <stdint.h>
#include <math.h>

#include "abs.h"
#include "controllers.h"
#include "pwm.h"

/*

Problem: “Implement an ABS state machine with states Idle, 
Apply, Release. Use pidControl to adjust pressure based on filtered speed. 
Transition: Idle if speed ≈ setpoint, Apply if speed < setpoint, 
Release if speed > setpoint.”

*/

const AbsState nextABSState[3][3] = {{IDLE, IDLE, IDLE},
                                  {APPLY, APPLY, APPLY},
                                  {RELEASE, RELEASE, RELEASE}};

static AbsEvent calcAbsEvent(float setpoint, float currentSpeedFilt);

AbsState absControl(float setpoint, float currentSpeedFilt, uint8_t* controlCmd, const PIDParams* params)
{
    float controlOutput;

    static AbsState current_state = IDLE;

    AbsEvent event;

    event = calcAbsEvent(setpoint, currentSpeedFilt);

    AbsState next_state = nextABSState[event][current_state];

    bool reset_pid = false;

    if (next_state != current_state)
    {
        reset_pid = true;
    }

    current_state = next_state;

    /* Calculate the PID control output periodically */
    controlOutput = pidControl(setpoint, currentSpeedFilt, reset_pid, params);

    /* Action - set the Control Command according to the AbsState */
    switch(next_state)
    {
        case IDLE:

            *controlCmd = (uint8_t)0;
            break;
            
        case APPLY:

                /* minus because increasing pressure results in reduced speed, 
               control output here means pressure gradient */
               *controlCmd = pidToPwm(-controlOutput);

        case RELEASE:
                /* minus because increasing pressure results in reduced speed, 
               control output here means pressure gradient */
               *controlCmd = pidToPwm(-controlOutput);
            break;

        default:
            *controlCmd = (uint8_t)0;
            break;
    }

    return next_state;

}

AbsEvent calcAbsEvent(float setpoint, float currentSpeedFilt)
{
    AbsEvent event;

    if ((setpoint - currentSpeedFilt) > ABS_WHL_SPEED_HYST)
    {
        event = SPD_UNDER_TARGET;
    }
    else if ((setpoint - currentSpeedFilt) < -ABS_WHL_SPEED_HYST)
    {
        event = SPD_OVER_TARGET;
    }
    else
    {
        event = SPD_ON_TARGET;
    }

    return event;
}
