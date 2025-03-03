# test_chassis.py
import numpy as np

# Simulated C functions (simplified)
def pid_control(setpoint, current, reset, kp=2.0, ki=0.1, kd=0.05, dt=0.01, max_out=100.0, min_out=-100.0):
    global integral, prev_error
    if 'integral' not in globals() or reset:
        integral = 0.0
        prev_error = 0.0
    error = setpoint - current
    if reset:
        prev_error = error
    derivative = (error - prev_error) / dt
    integral += error * dt
    output = kp * error + ki * integral + kd * derivative
    output = max(min(output, max_out), min_out)
    prev_error = error
    return output

def pid_to_pwm(pid_output):
    scaled = (pid_output + 100.0) * (255.0 / 200.0)
    return max(min(int(scaled + 0.5), 255), 0)

# Test data
steering_tests = [
    {"inputs": [-45, 0, 45, 30, -30, 0], "setpoint": 0.0,
     "expected_pwm": [255, 128, 0, 0, 255, 128], "expected_states": ["LEFT", "CENTER", "RIGHT", "RIGHT", "LEFT", "CENTER"]}
]

# Verification
for test in steering_tests:
    print(f"\nTesting Steering: Setpoint = {test['setpoint']}")
    for i, angle in enumerate(test["inputs"]):
        reset = (i == 0 or test["expected_states"][i] != test["expected_states"][i-1])
        pid_out = pid_control(test["setpoint"], angle, reset)
        pwm = pid_to_pwm(pid_out)
        state = "CENTER" if abs(test["setpoint"] - angle) <= 2 else ("LEFT" if test["setpoint"] - angle > 2 else "RIGHT")
        print(f"Angle: {angle:.2f}, State: {state}, PWM: {pwm}, Expected State: {test['expected_states'][i]}, Expected PWM: {test['expected_pwm'][i]}")
        assert state == test["expected_states"][i], f"State mismatch at {angle}"
        assert pwm == test["expected_pwm"][i], f"PWM mismatch at {angle}: got {pwm}, expected {test['expected_pwm'][i]}"
print("All tests passed!")