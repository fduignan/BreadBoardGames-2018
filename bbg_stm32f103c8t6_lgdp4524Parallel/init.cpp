// Initialization and interrupt vectors for the LPC1114 microcontroller
// No claims are made as to the suitability of this code for any 
// purpose whatsoever.  Use at your own risk!
// Does not include initialization for C++ global/static objects
// so probably best to use this with C projects for now.
// Latest version of this code can be found by visiting
// http://eleceng.dit.ie/frank and follow the links
// Author: Frank Duignan

#include "stm32f103.h"
#include "console.h"
void Reset_Handler(void);
void setup();
void loop();
void Reset_Handler(void);
void clock_init();
void Default_Handler(void);
void USART2_Handler(void);
void Systick_Handler(void);
// The following are 'declared' in the linker script
extern unsigned char  INIT_DATA_VALUES;
extern unsigned char  INIT_DATA_START;
extern unsigned char  INIT_DATA_END;
extern unsigned char  BSS_START;
extern unsigned char  BSS_END;
typedef void (*fptr)(void);

extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));


// the section "vectors" is placed at the beginning of flash 
// by the linker script
const fptr Vectors[] __attribute__((section(".vectors"))) ={
	(fptr)0x20005000, 	/* Top of stack (20k) */ 
	Reset_Handler,       /* Reset Handler */
	Default_Handler,	/* NMI */
	Default_Handler,	/* Hard Fault */
	Default_Handler,	/* MemManage */
	Default_Handler,	/* Bus Fault  */
	Default_Handler,	/* Usage Fault */
	Default_Handler,	/* Reserved */ 
	Default_Handler,	/* Reserved */
	Default_Handler,	/* Reserved */
	Default_Handler,	/* Reserved */
	Default_Handler,	/* SVCall */
	Default_Handler,	/* Reserved */
	Default_Handler,	/* Debug monitor */
	Default_Handler,	/* PendSV */
	Systick_Handler,    /* SysTick */	
/* External interrupt handlers follow */
	Default_Handler, 	/* 0: WWDG */
	Default_Handler, 	/* 1: PVD */
	Default_Handler, 	/* 2: Tamper */
	Default_Handler, 	/* 3: RTC */
	Default_Handler, 	/* 4: Flash */
	Default_Handler, 	/* 5: RCC */
	Default_Handler, 	/* 6: EXTI Line 0 */
	Default_Handler, 	/* 7: EXTI Line 1 */
	Default_Handler, 	/* 8: EXTI Line 2 */
	Default_Handler, 	/* 9: EXTI Line 3 */
	Default_Handler, 	/* 10: EXTI Line 4 */
	Default_Handler, 	/* 11: DMA1 Channel 1 */
	Default_Handler, 	/* 12: DMA1 Channel 2 */
	Default_Handler, 	/* 13: DMA1 Channel 3 */
	Default_Handler, 	/* 14: DMA1 Channel 4 */
	Default_Handler, 	/* 15: DMA1 Channel 5 */
	Default_Handler, 	/* 16: DMA1 Channel 6 */
	Default_Handler, 	/* 17: DMA1 Channel 7 */
	Default_Handler, 	/* 18: ADC1 and ADC2 */
	Default_Handler, 	/* 19: USB High Priority or CAN TX interrupts */
	Default_Handler, 	/* 20: USB Low Priority or CAN RX0 interrupts */
	Default_Handler, 	/* 21: CAN RX1 interrupt */
	Default_Handler, 	/* 22: CAN SCE interrupt */
	Default_Handler, 	/* 23: EXTI Line[9:5] interrupts */
	Default_Handler, 	/* 24: TIM1 Break interrupt */
	Default_Handler, 	/* 25: TIM1 Update interrupt */
	Default_Handler, 	/* 26: TIM1 Trigger and Commutation interrupts */
	Default_Handler, 	/* 27: TIM1 Capture Compare interrupt */
	Default_Handler, 	/* 28: TIM2 global interrupt */
    Default_Handler, 	/* 29: TIM3 global interrupt */
	Default_Handler, 	/* 30: TIM4 global interrupt */
	Default_Handler, 	/* 31: I2C1 event interrupt */
	Default_Handler, 	/* 32: I2C1 error interrupt */
	Default_Handler, 	/* 33: I2C2 event interrupt */
	Default_Handler, 	/* 34: I2C2 error interrupt */
	Default_Handler, 	/* 35: SPI1 global interrupt */
	Default_Handler, 	/* 36: SPI2 global interrupt */
	Default_Handler, 	/* 37: USART1 global interrupt */
	USART2_Handler, 	/* 38: USART2 global interrupt */
	Default_Handler, 	/* 39: USART3 global interrupt */
	Default_Handler, 	/* 40: EXTI Line[15:10] interrupts */
	Default_Handler, 	/* 41: RTC Alarms through EXTI line interrupt */
	Default_Handler, 	/* 42: Reserved */
	Default_Handler, 	/* 43: TIM8 Break interrupt */
	Default_Handler, 	/* 44: TIM8 Update interrupt */
	Default_Handler, 	/* 45: TIM8 Trigger and Commutation interrupts */
	Default_Handler, 	/* 46: TIM8 Capture Compare interrupt */
	Default_Handler, 	/* 47: ADC3 global interrupt */
	Default_Handler, 	/* 48: FSMC global interrupt */
	Default_Handler, 	/* 49: SDIO global interrupt */
	Default_Handler, 	/* 50: TIM5 global interrupt */
	Default_Handler, 	/* 51: SPI3 global interrupt */
	Default_Handler, 	/* 52: UART4 global interrupt */
	Default_Handler, 	/* 53: UART5 global interrupt */
	Default_Handler, 	/* 54: TIM6 global interrupt */
	Default_Handler, 	/* 55: TIM7 global interrupt */
	Default_Handler, 	/* 56: DMA2 Channel1 global interrupt */
	Default_Handler, 	/* 57: DMA2 Channel2 global interrupt */
	Default_Handler, 	/* 58: DMA2 Channel3 global interrupt */
	Default_Handler, 	/* 59: DMA2 Channel4 and Channel 5 global interrupt */
};
void clock_init()
{
// Set the clock to 72MHz
// An external 8MHz crystal is on board
/* Excerpt from reference manual
Several prescalers allow the configuration of the AHB frequency, the high speed APB
(APB2) and the low speed APB (APB1) domains. The maximum frequency of the AHB and
the APB2 domains is 72 MHz. The maximum allowed frequency of the APB1 domain is
36 MHz. The SDIO AHB interface is clocked with a fixed frequency equal to HCLK/2
The RCC feeds the Cortex ® System Timer (SysTick) external clock with the AHB clock
(HCLK) divided by 8. The SysTick can work either with this clock or with the Cortex ® clock
(HCLK), configurable in the SysTick Control and Status Register. The ADCs are clocked by
the clock of the High Speed domain (APB2) divided by 2, 4, 6 or 8.
    */
    RCC->CR &= ~BIT18; // clear HSEBYP 
    RCC->CR |= BIT16;  // set (turn on) HSE
    while ( (RCC->CR | BIT17)         == 0); // wait for HSE Ready

    // set PLL multiplier to 9 giving 72MHz as PLL output
    RCC->CFGR &= ~(BIT21);
    RCC->CFGR |= BIT20 | BIT19 | BIT18;      
    // Limit APB1 to 36MHz
    RCC->CFGR |= BIT10;
    RCC->CFGR &= ~(BIT9+BIT8);
    RCC->CFGR |= BIT16; // switch PLL clock source to HSE
    
    RCC->CR |= BIT24;         // turn on PLL
    while ( (RCC->CR | BIT24) == 0); // wait for PLL lock
    // Insert wait states for FLASH : need 2 at 72MHz
    FLASH->ACR |= BIT1;
    FLASH->ACR &= ~(BIT2+BIT0);
    // Turn on pre-fetch buffer to speed things up
    FLASH->ACR |= BIT4;
    // Switch to PLL output for system clock
    RCC->CFGR &= BIT0;
    RCC->CFGR |= BIT1;

}
void init_array()
{
    // This function calls constructors for global and static objects
    uint32_t count;
    uint32_t i;
    
    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
        __preinit_array_start[i] ();
    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i] (); 
}
void Reset_Handler()
{
    // do global/static data initialization
    unsigned char *src;
    unsigned char *dest;
    unsigned len;
    src= &INIT_DATA_VALUES;
    dest= &INIT_DATA_START;
    len= &INIT_DATA_END-&INIT_DATA_START;
    while (len--)
        *dest++ = *src++;
    // zero out the uninitialized global/static variables
    dest = &BSS_START;
    len = &BSS_END - &BSS_START;
    while (len--)
        *dest++=0;
    clock_init();
    init_array();
    Console.begin();    
    setup();
    while(1)
    {
        loop();
    }
}

void Default_Handler()
{
    while(1);
}
