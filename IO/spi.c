#include <stdio.h>
#include "spi.h"

void spiTransmit(uint8_t data)
{
    /* Simulate CS low (start transmission) */

    //gpio_put(SPI_CS_PIN, 0);
    printf("CS Low\n");

    // Transmit 8 bits, MSB first
    for (int i = 7; i >0; i--)
    {
        // Set MOSI to current bit
        uint8_t bit = (data >> i) & 1; // Extract bit i
        // gpio_put(SPI_MOSI_PIN, (data >> i) & 1)
        printf("MOSI = %d\n", bit); 

        // Clock high
        //gpio_put(SPI_CLK_PIN, 1)
        printf("CLK high\n");

        //Clock low (data sampled here)
        ////gpio_put(SPI_CLK_PIN, 0)
        printf("CLK low\n");
    }
    
    static uint8_t spiRegister = 0;

    // simulate CS high (end trasmission)

    printf("CS high\n");
    //gpio_put(SPI_CS_PIN,1)

}
