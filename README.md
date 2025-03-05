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

This project supports both PC simulation and Raspberry Pi Pico 2 deployment, with VS Code tasks and launch configurations for streamlined development. Follow the steps below based on your target environment.

### PC Simulation

#### Building
Use VS Code tasks to compile for PC simulation in either Debug or Release mode:

1. **Open in VS Code**:
   - Ensure you have the recommended extensions installed (see VS Code Extensions below).

2. **Build for Debug**:
   - Run the “Build (PC Debug)” task:
     ```bash
     Ctrl+Shift+B (or Cmd+Shift+B on macOS) and select "Build (PC Debug)"
     ```
   This executes:
     ```bash
     mkdir -p build_pc && cd build_pc && cmake -DCMAKE_BUILD_TYPE=Debug -DPC_BUILD=ON .. && make -j4
     ```
   Generates build_pc/ChassisControlsPC for debugging.

3. **Build for Release**:
   - Run the “Build (PC Release)” task:
     ```bash
     Ctrl+Shift+B and select "Build (PC Release)"
     ```
   This executes:
     ```bash
     mkdir -p build_pc && cd build_pc && cmake -DCMAKE_BUILD_TYPE=Release -DPC_BUILD=ON .. && make -j4
     ```
   Generates build_pc/ChassisControlsPC for optimized execution.

4. **Clean**:
   - Run the “Clean” task to remove build artifacts:
     ```bash
     Ctrl+Shift+B and select "Clean"
     ```
   This executes:
     ```bash
     rm -rf build build_pc
     ```

#### Running
- **Run PC Simulation**:
  - Use the “Run (PC)” task after a Release build:
    ```bash
    Ctrl+Shift+B and select "Run (PC)"
    ```
  This executes:
    ```bash
    ./build_pc/ChassisControlsPC
    ```
  Outputs ABS and steering test sequences to the console.

#### Debugging
- **Debug PC Build**:
  - Set breakpoints in ChassisControls.c or related files and launch the “Debug PC Build” configuration:
    ```bash
    Press F5 and select “Debug PC Build”
    ```
  This uses the cppdbg debugger with gdb, targeting build_pc/ChassisControlsPC.
  Requires a prior “Build (PC Debug)” task.

### Raspberry Pi Pico 2

#### Setup
- **Install Pico SDK and Tools**:
  - Follow Getting Started with Pico.
  - Set PICO_SDK_PATH (e.g., ${env:HOME}/.pico-sdk/sdk/2.1.1 on Linux/macOS, ${env:USERPROFILE}/.pico-sdk/sdk/2.1.1 on Windows).
  - Ensure PICO_TOOLCHAIN_PATH, CMake, ARM GCC, and OpenOCD are installed as per your settings.json and tasks.json.

#### Building
- **Build for Pico**:
  - Open in VS Code, ensure the “Pico” configuration is active in c_cpp_properties.json.
  - Run the “Build (Pico)” task:
    ```bash
    Ctrl+Shift+B and select "Build (Pico)"
    ```
  This executes:
    ```bash
    mkdir -p build && cd build && cmake .. && ${env:HOME}/.pico-sdk/ninja/v1.12.1/ninja -j4
    ```
  (or ${env:USERPROFILE}/.pico-sdk/ninja/v1.12.1/ninja.exe on Windows).
  Generates build/ChassisControls.uf2 for flashing.

#### Flashing
- **Flash to Pico**:
  - Run the “Flash” task:
    ```bash
    Ctrl+Shift+B and select "Flash"
    ```
  This executes:
    ```bash
    ${env:HOME}/.pico-sdk/openocd/0.12.0+dev/openocd.exe -s ${env:HOME}/.pico-sdk/openocd/0.12.0+dev/scripts -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000; program \"${workspaceFolder}/build/ChassisControls.uf2\" verify reset exit"
    ```
  (or Windows path ${env:USERPROFILE}).
  Connect the Pico 2 in BOOTSEL mode, then copy ChassisControls.uf2 to the Pico drive.

#### Debugging
- **Pico Debugging**:
  - Use one of these configurations in launch.json:

    *Pico Debug (Cortex-Debug):*
    - Press F5, select “Pico Debug (Cortex-Debug)” (uses OpenOCD internally).
    - Requires “Build (Pico)” and “Flash” tasks first.

    *Pico Debug (Cortex-Debug with external OpenOCD):*
    - Launch OpenOCD manually, then select this configuration.

    *Pico Debug (C++ Debugger):*
    - Press F5, select “Pico Debug (C++ Debugger)” (uses cppdbg with OpenOCD).
    - Requires “Flash” task.
    - Set breakpoints in ChassisControls.c or related files.
    - Uses SVD for register visualization, targeting build/ChassisControls.uf2.

Note: The current ChassisControls.c runs simulation-style tests. For Pico, modify ChassisControls.c to integrate with real sensor inputs (e.g., ADC for angles/speeds) and PWM outputs (e.g., GPIO for actuators).

### VS Code Extensions
To fully utilize this project in VS Code, install these recommended extensions (as per extensions.json):

- marus25.cortex-debug (for ARM debugging)  
- ms-vscode.cpptools (C/C++ IntelliSense)  
- ms-vscode.cpptools-extension-pack (C/C++ tools)  
- ms-vscode.vscode-serial-monitor (serial communication)  
- raspberry-pi.raspberry-pi-pico (Pico-specific tools)

Install via the VS Code Extensions view or by adding to your settings.json.

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

