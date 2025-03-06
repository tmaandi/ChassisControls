# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add option for PC build
option(PC_BUILD "Build for PC instead of Pico" OFF)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

if(PC_BUILD)
    project(ChassisControlsPC C)
    add_definitions(-DPC_BUILD)
else()
    # Initialize Pico SDK properly
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
    include(pico_sdk_import.cmake)
    project(ChassisControls C CXX ASM)
    pico_sdk_init()
endif()

# Common source files
set(SOURCES 
    ChassisControls.c
    ABS/abs.c
    AC/ac.c
    FM/fault.c
    IO/pwm.c
    IO/adc.c
    IO/can.c
    IO/spi.c
    IO/uart.c
    SA/sensors.c
    SC/controllers.c
    SC/filters.c
    STEER/steer.c
)

if(PC_BUILD)
    add_executable(ChassisControlsPC ${SOURCES})
    target_include_directories(ChassisControlsPC PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/ABS
        ${CMAKE_CURRENT_LIST_DIR}/AC
        ${CMAKE_CURRENT_LIST_DIR}/FM
        ${CMAKE_CURRENT_LIST_DIR}/IO
        ${CMAKE_CURRENT_LIST_DIR}/SA
        ${CMAKE_CURRENT_LIST_DIR}/SC
        ${CMAKE_CURRENT_LIST_DIR}/STEER
    )
    # Add math library for PC build
    target_link_libraries(ChassisControlsPC m)
else()
    add_executable(ChassisControls ${SOURCES})
    target_include_directories(ChassisControls PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/ABS
        ${CMAKE_CURRENT_LIST_DIR}/AC
        ${CMAKE_CURRENT_LIST_DIR}/FM
        ${CMAKE_CURRENT_LIST_DIR}/IO
        ${CMAKE_CURRENT_LIST_DIR}/SA
        ${CMAKE_CURRENT_LIST_DIR}/SC
        ${CMAKE_CURRENT_LIST_DIR}/STEER
    )
    # Add pico-specific configurations
    pico_set_program_name(ChassisControls "ChassisControls")
    pico_set_program_version(ChassisControls "0.1")
    pico_enable_stdio_uart(ChassisControls 0)
    pico_enable_stdio_usb(ChassisControls 0)
    target_link_libraries(ChassisControls pico_stdlib)
    pico_add_extra_outputs(ChassisControls)
endif()

