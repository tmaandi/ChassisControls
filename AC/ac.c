/*

“Write a C function updateStatusRegister that manages a 16-bit status register
for an actuator system. Bits represent fault states, actuator modes, 
and sensor health. Use bitwise ops to set, clear, and check flags.”

Specs:
Register layout:
Bit 0: Overcurrent fault (1 = faulted).
Bit 1: Overtemp fault (1 = faulted).
Bit 2-3: Actuator mode (00 = off, 01 = forward, 10 = reverse, 11 = hold).
Bit 4: Wheel speed sensor fault (1 = faulted).
Bits 5-15: Reserved (0).
Inputs: uint16_t* reg (pointer to register), uint8_t bit_pos, uint8_t value (0 or 1 for single bits, 0-3 for mode).
Output: None (modifies *reg in place).

*/

#include <stddef.h>
#include "ac.h"

bool updateStatusRegister(uint16_t* reg, uint8_t bit_pos, uint8_t value)
{
    // Safety Checks

    if (reg == NULL || bit_pos > 15) return false; // Invalid pointer or position

    if (bit_pos == MODE_POS)
    {
        // handle 2-bits
        if (value > 3) return false; //invalid value
        *reg &= ~MODE_MASK; // clear bits 2-3
        *reg |= (value << MODE_POS); // Set new mode
    }
    else if (bit_pos == FAULT_OVERCURRENT || bit_pos == FAULT_OVERTEMP || bit_pos == FAULT_WHEEL_SPEED)
    {
        if (value > 1) return false; // invalid value
        if (value)
        {
         *reg |= (1U << bit_pos); // Set bit
        }
        else
        {
            *reg &= ~(1U << bit_pos); // Clear bit          
        }
    }
    else
    {
        return false; // Invalid Bit Position
    }

    return true;

    // Bits 5-15 reserved 
}