
#include <math.h>


#include "filters.h"

#include "abs.h"

/*

Problem: “Implement an ABS state machine with states Idle, 
Apply, Release. Use pidControl to adjust pressure based on filtered speed. 
Transition: Idle if speed ≈ setpoint, Apply if speed < setpoint, 
Release if speed > setpoint.”

*/

const AbsState nextState[3][3] = {{IDLE, IDLE, IDLE},
                                  {APPLY, APPLY, APPLY},
                                  {RELEASE, RELEASE, RELEASE}};

static AbsEvent calcAbsEvent(float setpoint, float currentSpeedFilt);

AbsState absControl(float setpoint, float currentSpeedFilt, float* controlCmd, const PIDParams* params)
{
    float controlOutput;

    static AbsState current_state = IDLE;

    AbsEvent event;

    event = calcAbsEvent(setpoint, currentSpeedFilt);

    AbsState next_state = nextState[event][current_state];

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

            *controlCmd = 0.0f;
            break;
            
        case APPLY:

                /* minus because increasing pressure results in reduced speed, 
               control output here means pressure gradient */
               *controlCmd = -controlOutput;

        case RELEASE:
                /* minus because increasing pressure results in reduced speed, 
               control output here means pressure gradient */
               *controlCmd = -controlOutput;
            break;

        default:
            *controlCmd = 0.0f;
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
