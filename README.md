# Chassis Control Simulation and Verification

## Overview
This project is a comprehensive simulation and testing framework for chassis control systems, focusing on Anti-lock Braking System (ABS) and steering control algorithms written in C. It was developed as part of a personal preparation for an embedded systems engineering interview targeting chassis control software integration (e.g., braking, steering). The codebase includes state machines, PID controllers, PWM signal generation, and fault detection, all designed to mirror real-world automotive embedded applications.

While the primary goal was to test and validate these algorithms on a PC environment, the project is built atop the Raspberry Pi Pico 2 toolchain, making it fully compatible for compilation and deployment on the Raspberry Pi Pico 2 microcontroller. Configuration files (`tasks.json` and `launch.json`) support both PC simulation and Pico hardware builds, allowing others to adapt it for real embedded use cases.

This repository aims to serve as a learning resource and starting point for anyone interested in embedded systems, automotive controls, or interview prep in similar domains.

## Features
- **ABS State Machine:** Controls braking with states (IDLE, APPLY, RELEASE) using a 2D transition table, simulating normally-open isolation valves.
- **Steering State Machine:** Manages steering with states (LEFT, CENTER, RIGHT), adjusting actuator position via PWM based on angle error.
- **PID Controller:** Implements a discrete PID algorithm with integral windup protection and reset functionality, reusable across ABS and steering.
- **PWM Signal Generation:** Converts PID outputs (-100 to 100) to PWM duty cycles (0-255) for actuator control.
- **Fault Detection:** Identifies stuck or out-of-range sensor data (e.g., wheel speed, steering angle) with minimal computational overhead.
- **Python Verification:** Includes a Python script (`Tests/test_chassis.py`) to simulate and validate the C algorithms with test data, mimicking unit testing workflows.

## Prerequisites
### For PC Simulation
- **GCC:** Any C compiler (e.g., gcc via MinGW on Windows, or native on Linux/Mac).
- **Python 3:** For running the verification script (`Tests/test_chassis.py`).
- **VS Code:** Recommended for building and debugging with provided `tasks.json` and `launch.json`.

### For Raspberry Pi Pico 2
- **Pico SDK:** Install the Raspberry Pi Pico 2 SDK (see Getting Started with Pico).
- **CMake:** For generating Pico build files.
- **ARM GCC:** Cross-compiler for Pico (arm-none-eabi-gcc).
- **OpenOCD:** For flashing and debugging (optional).

## Project Structure

- `ChassisControls/`
  - `.vscode/`
    - `tasks.json` # Build tasks for PC and Pico
    - `launch.json` # Debug configs for PC and Pico
  - `ABS/`
    - `abs.c` # ABS state machine implementation
    - `abs.h` # ABS header
  - `FM/`
    - `fault.c` # Fault detection implementation
    - `fault.h` # Fault header
  - `SC/`
    - `controllers.c` # PID controller implementation
    - `controllers.h` # PID header
  - `filters.c` # Moving average filter implementation
  - `filters.h` # Filter header
  - `pwm.c` # PWM conversion implementation
  - `pwm.h` # PWM header
  - `STEER/`
    - `steer.c` # Steering state machine implementation
    - `steer.h` # Steering header
  - `Tests/`
    - `test_chassis.py` # Python verification script
  - `ChassisControls.c` # Main entry point with test sequences
  - `.gitignore` # Git ignore file
  - `CMakeLists.txt` # CMake configuration for Pico
  - `pico_sdk_import.cmake` # Pico SDK import

## Building and Running

### PC Simulation

Compile:
```bash
gcc -o ChassisControls ChassisControls.c ABS/abs.c FM/fault.c SC/controllers.c filters.c pwm.c STEER/steer.c -lm
```

Run:
```bash
./ChassisControls
```
Outputs ABS and steering test sequences to console.

Verify with Python:
```bash
python Tests/test_chassis.py
```
Simulates steering inputs and validates PWM/states.

### Raspberry Pi Pico 2

Setup Pico SDK:
Follow Pico SDK setup.
Set PICO_SDK_PATH environment variable.

Configure:
Open in VS Code, select "Pico" build task in tasks.json.

Build:
Run CMake: Configure and CMake: Build in VS Code.
Generates ChassisControls.uf2 in the build folder.

Flash:
Copy ChassisControls.uf2 to the Pico 2 in BOOTSEL mode.

Debug:
Use launch.json with OpenOCD for on-device debugging.

Note: The current ChassisControls.c runs simulation-style tests. For Pico, modify ChassisControls.c to integrate with real sensor inputs (e.g., ADC for angles/speeds) and PWM outputs (e.g., GPIO for actuators).

## Usage

ABS: Simulates wheel speed control with [10, 8, 6, 6, 8, 10, 14, 16, 14].  
Steering: Simulates angle control with [-45, 0, 45, 30, -30, 0].  
Fault Detection: Tests stuck ([10, 10, 10]) or out-of-range ([200, 200]) sensors.  
Python: Validates steering logic—extend for ABS/fault tests.

## Contributing

Feel free to fork, tweak, or add features! Pull requests welcome—especially for:
- Real Pico hardware integration (e.g., ADC, PWM).
- More test cases in Tests/test_chassis.py.
- Optimized PID tuning for specific actuators.

## License

MIT License—free to use, modify, and share. See LICENSE for details.

## Acknowledgments

Built with guidance from Grok (xAI)—thanks for the assist!  
Inspired by real-world chassis control challenges.

