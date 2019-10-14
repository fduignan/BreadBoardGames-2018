// spi.h for stm32f103c8t6
// Author: Frank Duignan.  
// Updates posted on http://ioprog.com 

#include <stdint.h>
#include "stm32f103.h"
#include "spi.h"
void initSPI(void)
{
	int drain_count,drain;
	
	RCC->APB1ENR |= (1 << 14);		// turn on SPI2 	
	
	
	// GPIOB bits 12 and 15 are used for SPI2.  Not using MISO or NSS for this application
	RCC->APB2ENR |= (1 << 3);      	// enable port B
    
    GPIOB->CRH &= (0x0f0fffff); // select alternate function with push pull for bits 12,13 and 15
    GPIOB->CRH |= (0xb0b00000); // atlernate function output mode, 50MHz

	// Now configure the SPI interface
	drain = SPI2->SR;				// dummy read of SR to clear MODF	
	// enable SSM, set SSI, enable SPI, PCLK/2, LSB First Master, Clock = 1 when idle, second clock transition is active
	SPI2->CR1 = (1 << 9)+(1 << 8)+(1 << 6)+(1 << 2)+(1 << 1)+(1 << 0);	
   
    for (drain_count = 0; drain_count < 32; drain_count++)
		drain = transferSPI8(0x00);
}

uint8_t transferSPI8(uint8_t data)
{
    unsigned Timeout = 1000000;
    uint8_t ReturnValue;
    uint8_t *preg=(uint8_t*)&SPI2->DR;
	
    while (((SPI2->SR & (1 << 7))!=0)&&(Timeout--));
    *preg = data;
    Timeout = 1000000;
    while (((SPI2->SR & (1 << 7))!=0)&&(Timeout--));            
	ReturnValue = *preg;
	
    return ReturnValue;
}

uint16_t transferSPI16(uint16_t data)
{
    unsigned Timeout = 1000000;
    uint16_t ReturnValue;    
    transferSPI8(data >> 8);
    transferSPI8(data & 0xff);
    return 0;
    while (((SPI2->SR & (1 << 7))!=0)&&(Timeout--));
    SPI2->DR = data;
    Timeout = 1000000;
    while (((SPI2->SR & (1 << 7))!=0)&&(Timeout--));        
	ReturnValue = SPI2->DR;
	
    return ReturnValue;
}
