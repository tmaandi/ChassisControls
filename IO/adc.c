/*
“Implement adcInterruptHandler to sample ADC every 1ms, update actuatorFaultState, 
and call detectActuatorFault. Simulate ADC with a counter.”
*/

#include "fault.h"
#include "pwm.h"

volatile uint16_t latestAdcValue = 0;

void adcInterruptHandler(void)
{
    /*
    simulate ADC
    */
    static uint16_t sim_adc = 700;

    sim_adc += 10; // Count/drift up

    if (sim_adc > 1023)
    {
        sim_adc = 700; // Reset
    }

    latestAdcValue = sim_adc;

    detectActuatorFault(&actuatorFaultState, sim_adc);
}