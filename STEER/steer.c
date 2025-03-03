/*
Steering State Machine Challenge
Problem
“Implement a steering control state machine with states LEFT, CENTER, and RIGHT. Use pidControl to adjust steering angle based on a target angle (setpoint) and current filtered angle (from movingAverage). Convert the PID output to a PWM duty cycle (0 to 255) to drive a steering actuator, where:

PWM = 0 → full left,
PWM = 128 → center,
PWM = 255 → full right. Transitions should align with the angle error: steer left if too far right, right if too far left, and center when on target.”
Specs
Inputs:
float setpoint: Target steering angle (e.g., -45° to 45°).
float currentAngle: Raw steering angle from sensor.
Output: uint8_t controlCmd (PWM duty cycle, 0-255).
States: LEFT, CENTER, RIGHT.
Events: Based on angle error (setpoint - filteredAngle):
ANGLE_ON_TARGET: Near setpoint.
ANGLE_LEFT: Too far right (needs left turn).
ANGLE_RIGHT: Too far left (needs right turn).
Constraints:
Use movingAverage for filtering.
Use pidControl with reset.
No if-else mess—use a 2D transition table.
*/

#include "steer.h"
#include "controllers.h"
#include "pwm.h"

const SteeringState nextSteerState[3][3] = {{CENTER, CENTER, CENTER},
                                       {LEFT, LEFT, CENTER},
                                       {CENTER, RIGHT, RIGHT}}; 

static SteeringEvent calcSteerEvent(float setpoint, float currentAngleFilt);                                       

SteeringState steeringControl(float setpoint, float currentAngleFilt, uint8_t* controlCmd, const PIDParams* params)
{
    static SteeringState currentState = CENTER;

    SteeringEvent event;

    event = calcSteerEvent(setpoint, currentAngleFilt);

    float pidControlOutput;

    SteeringState nextState = nextSteerState[event][currentState];

    bool resetPID = false;

    if (nextState != currentState)
    {
        resetPID = true;
    }

    currentState = nextState;

    pidControlOutput = pidControl(setpoint, currentAngleFilt, resetPID, params);

    if (currentState == CENTER) 
    {
        *controlCmd = 128;
    } 
    else 
    {
        *controlCmd = pidToPwm(pidControlOutput);
    }
    
    return currentState;

}

SteeringEvent calcSteerEvent(float setpoint, float currentAngleFilt)
{
    float steer_error;

    SteeringEvent event;

    event = ANGLE_ON_TARGET;

    steer_error = (setpoint - currentAngleFilt);

    if (steer_error > STEERING_ANG_HYST) // Steering too much to right, turn left
    {
        event = ANGLE_LEFT;
    }
    else if (steer_error < -STEERING_ANG_HYST) // steering too much to left, turn right
    {
        event = ANGLE_RIGHT;
    }
    else
    {
        event = ANGLE_ON_TARGET;
    }

    return event;
}