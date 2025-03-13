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
#include "i2c.h"
#include "pwm.h"
#include "sensors.h"
#include "spi.h"
#include "steer.h"
#include "tm.h"
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

        adcInterruptHandler();

        printf("\nActuator Fault Detected: %d\n\n", actuatorFaultState.faulted);

        printf("Before PWM Limit: %d\n",controlCmdABS);

        /* Test PWM Limit */
        controlCmdABS = limitPwmDuty(controlCmdABS);

        printf("After PWM Limit: %d\n",controlCmdABS);

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

    /* Test monitorActuatorFault */
    uint8_t actuatorFaultFlag = 0;
    uint16_t actuatorCurrent = 4100;

    monitorActuatorFault(actuatorCurrent, &actuatorFaultFlag);

    printf("actuatorFaultFlag: %d\n", actuatorFaultFlag);

    /* void parseCanMessage(uint8_t* buffer) */
    uint8_t bufferToCANParse[] = {0x01, 0x23, 0xAA, 0xBB, 0, 0, 0, 0, 0, 0};
    
    parseCanMessage(bufferToCANParse);

    /* Test I2C */
    printf("I2C Test \n");

    uint8_t presSensorData[] = {0x0A, 0xF0};
    uint16_t pressure = readBrakePressureSensor(0x1A, 0x05, presSensorData);
    printf("Pressure: %u kPa\n", pressure);
    
    /* Test Brake Actuator Logger */
    AC_LOGGER_TYPE brakeAcState = {2500, 
                                   128,
                                   1};

    logBrakeActuatorState(&brakeAcState);   

    /* Temperature Monitoring */
    printf("Monitoring Temp...\n");

    monitorTemp();

    /* LPF Test */
    LOW_PASS_FILT_STATE_TYPE filter;

    initLowPassFilter(&filter);

    uint16_t readings[] = {12000, 13000, 11000};  // mA
    
    for (int i = 0; i < 3; i++) {
        uint16_t filtered = lowPassFilter(&filter, 100, 1000, readings[i]);  // 100 Hz cutoff, 1 kHz sample
        printf("Input: %u, Filtered: %u mA\n", readings[i], filtered);
    }

    return 0;
}
