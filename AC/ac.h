#ifndef AC_H
#define AC_H

#include <stdint.h>
#include <stdbool.h>

#include "ac_logger.h"

#define FAULT_OVERCURRENT 0
#define FAULT_OVERTEMP 1
#define MODE_POS 2
#define FAULT_WHEEL_SPEED 4
#define MODE_MASK 0x000C  // Bits 2-3 (0b00001100)


#define NEWTON_RES 16
#define RPM_RES 1

bool updateStatusRegister(uint16_t* reg, uint8_t bit_pos, uint8_t value);

#endif // AC_H