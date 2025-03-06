#ifndef CAN_H
#define CAN_H


#include <stdint.h>

#define CAN_ID_MAX 0x7FF  // 11-bit max
#define CAN_DLC 8         // 8 bytes

void canTransmit(uint16_t id, uint8_t* data);



#endif // CAN_H




