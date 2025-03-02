
/*

Problem: “Implement an ABS state machine with states Idle, 
Apply, Release. Use pidControl to adjust pressure based on filtered speed. 
Transition: Idle if speed ≈ setpoint, Apply if speed < setpoint, 
Release if speed > setpoint.”

*/

typedef enum {IDLE,             // 0
              APPLY,            // 1
              RELEASE}          // 2
              AbsState;