/*
Thermocouple: monitorTemp()—NTC or N-Type, Trip at 125°C
Purpose
Monitor MOSFET or motor temperature using an NTC thermistor (simpler than N-type thermocouple for this demo) 
and trip at 125°C.
Assumptions
NTC: 10 kΩ at 25°C, β = 3950 (common automotive spec).
ADC: 0-5 V, 12-bit (0-4095), 5 V supply via voltage divider.
Trip: 125°C (~1 kΩ for NTC).
*/

#include <stdio.h>

#include "tm.h"

#define ADC_MAX 4095        // 12-bit ADC
#define TEMP_TRIP 125       // °C trip point
#define BETA 3950           // NTC beta coefficient
#define R0 10000            // NTC resistance at 25°C
#define T0 298.15           // 25°C in Kelvin

volatile uint8_t fault_flag = 0;  // Shared fault flag

// Simulated ADC read (replace with S32K ADC driver)
uint16_t adc_read_temp(void)
{
    // For demo: Assume NTC at 125°C (~1 kΩ), R_div = 10 kΩ
    // V_out = 5 * R_ntc / (R_ntc + R_div), at 1 kΩ: 0.454 V
    return (uint16_t)(0.454 * ADC_MAX / 5.0); // ~ 373

}

// Calculate temperature from ADC value (NTC in divider with 10 kΩ)
float calc_temp(uint16_t adc_val)
{
        float v_adc = ((float)adc_val * 5.0)/ADC_MAX; // Voltage

        float r_ntc = (v_adc * 10000) / (5.0 - v_adc); // NTC resistance

        float temp_k = 1.0 /((1.0 / T0) + (1.0 / BETA) * log(r_ntc / R0)); // Kelvin

        return temp_k - 273.15; // deg C
}


void monitorTemp(void)
{
    uint16_t adc_val = adc_read_temp();
    float temp = calc_temp(adc_val);

    if (temp >= TEMP_TRIP) {
        fault_flag |= (1 << 1);  // Bit 1 for temp fault
        printf("Temp Fault: %.1f°C exceeds 125°C\n", temp);
        // Add: PWM disable, CAN fault message
    } else {
        printf("Temp: %.1f°C\n", temp);
    }
}