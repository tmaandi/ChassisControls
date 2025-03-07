#include <stdio.h>
#include <stdint.h>

#ifndef PC_BUILD
#include "pico/stdlib.h"
#endif

#include "controllers.h"
#include "filters.h"

#include "abs.h"
#include "ac.h"
#include "adc.h"
#include "can.h"
#include "fault.h"
#include "pwm.h"
#include "sensors.h"
#include "spi.h"
#include "steer.h"
#include "uart.h"

const PIDParams absParams = {  1.00F,   /* KP */  
                               0.10F,   /* KI */  
                               0.05F,   /* KD */ 
                               0.01F,   /* DT */ 
                               50.00F,  /* INTEGRAL_MAX */ 
                               100.00F, /* OUTPUT_MAX */
                              -100.0F   /* OUTPUT_MIN */ 
                            };

const PIDParams steerParams = { 2.0F, 
                                0.1F, 
                                0.05F, 
                                0.01F, 
                                50.0F, 
                               100.0F, 
                              -100.0F};

int main() {

    float speeds[] = {10, 8, 6, 6, 8, 10, 14, 16, 14};

    int num_speeds = sizeof(speeds)/sizeof(float);

    float setpointABS = 10.0F;

    /* Normal */
    float steering_angles[] = {-45, 0, 45, 30, -30, 0};
    /* Stuck */
    // float steering_angles[] = {10, 10, 10, 10, 10, 10};
    // /* Out-of-Range */
    // float steering_angles[] = {200, 200};

    int num_steer_angles = sizeof(steering_angles)/sizeof(float);

    float setpointSteer = 0.0F;

    AbsState abs_state = IDLE;

    SteeringState steer_state = CENTER;

    uint8_t controlCmdABS = 0;
    uint8_t controlCmdSteer = 0;

    float filtered_whl_spd;
    float filtered_steer_angle;

    float actuatorOutput;

    const char* absStateStr;
    const char* steerStateStr;

    for (int i = 0; i < num_speeds; i++) 
    {

        filtered_whl_spd = movingAverage(&WhlSpeedFiltState, speeds[i]);

        abs_state = absControl(setpointABS, filtered_whl_spd, &controlCmdABS, &absParams);

        switch (abs_state) {
            case IDLE:
            absStateStr = "IDLE";
            break;
            case APPLY:
            absStateStr = "APPLY";
            break;
            case RELEASE:
            absStateStr = "RELEASE";
            break;
            default:
            absStateStr = "UNKNOWN";
            break;
        }

        // Testing readWheelSpeed()
        // filtered_whl_spd = readWheelSpeed(adc)

        if (filtered_whl_spd < SPEED_THRESHOLD)
        {
            controlCmdABS = 0;
        }

        adcInterruptHandler();

        printf("\nActuator Fault Detected: %d\n\n", actuatorFaultState.faulted);

        actuatorOutput = pwmActuator(controlCmdABS, &actuatorFaultState);
        
        printf("Setpoint: %.2f, Filtered Speed: %.2f, AbsState: %s, Control: %d\n", setpointABS, filtered_whl_spd, absStateStr, controlCmdABS);

    }

    for (int i = 0; i < num_steer_angles; i++)
    {
        /* Steering Control */
        filtered_steer_angle = movingAverage(&SteerAngFiltState, steering_angles[i]);

        steer_state = steeringControl(setpointSteer, filtered_steer_angle, &controlCmdSteer, &steerParams);

        switch (steer_state) {
            case CENTER:
            steerStateStr = "CENTER";
            break;
            case LEFT:
            steerStateStr = "LEFT";
            break;
            case RIGHT:
            steerStateStr = "RIGHT";
            break;
            default:
            steerStateStr = "UNKNOWN";
            break;
        }
        
        printf("Setpoint: %.2f, Filtered Steering Angle: %.2f, SteerState: %s, Control: %d\n", setpointSteer, filtered_steer_angle, steerStateStr, controlCmdSteer);

    }

    /* Steering Fault Management */

    bool fault = detectFault(steering_angles, 5, -50.0f, 50.0f);

    printf("\nFault Detected: %b\n\n",fault);

    /* Test Bit masking */

    uint16_t reg = 0;

    // Test 1: Set overcurrent fault
    updateStatusRegister(&reg, FAULT_OVERCURRENT, 1);
    printf("Overcurrent: 0x%04X (expect 0x0001)\n", reg);  // 0b0001

    // Test 2: Set mode to reverse
    updateStatusRegister(&reg, MODE_POS, 2);  // 10 = reverse
    printf("Reverse mode: 0x%04X (expect 0x0009)\n", reg);  // 0b0101

    // Test 3: Clear overcurrent
    updateStatusRegister(&reg, FAULT_OVERCURRENT, 0);
    printf("Clear overcurrent: 0x%04X (expect 0x0008)\n", reg);  // 0b0100

    // Edge case: Invalid value for fault
    updateStatusRegister(&reg, FAULT_OVERTEMP, 2);  // Ignored
    printf("Invalid value: 0x%04X (expect 0x0008)\n", reg);

    /* SPI Test */
    spiTransmit(0xA5);

    /* UART Test */
    // gpio_init(UART_TX_PIN); 
    // gpio_set_dir(UART_TX_PIN, GPIO_OUT);
    uartTransmit(0x41);
    
    /* CAN Test */
    uint16_t id = 0x123;
    uint8_t data[8] = {0x01, 0x02, 0x03, 0x04,0x05, 0x06, 0x07, 0x08};

    canTransmit(id, data);
    
    return 0;
}
