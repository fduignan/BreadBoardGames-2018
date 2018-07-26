#include "controller.h"
#include "stm32f103.h"
#include <stdint.h>
void controller::begin()
{
     
    // Low level I/O port initialisation
    // Turn on GPIO A, B and C
    RCC->APB2ENR |= BIT2 + BIT3 + BIT4;
    // Make all bits connected to the display outputs to start with
    // Note: PA15, PB4, PB3 are mapped to JTAG functionality at reset.    
    // Need to restore these to port operation first
    RCC->APB2ENR |= BIT0; // enable clock for AFIO latches
    AFIO->MAPR = 0x02000000; // turn off JTAG but leave SWD on    
    // Port A bits 8,9,10,11,15    
    GPIOA->CRH = (GPIOA->CRH & 0x0fff0000) | 0x10001111;
    // Port B bits 3,4,5,6,12,13,14,15
    GPIOB->CRL = (GPIOB->CRL & 0xf0000fff) | 0x01111000;
    GPIOB->CRH = (GPIOB->CRH & 0x0000ffff) | 0x11110000;
    
    // Button layout:
    // B1   : Left
    // B11  : Fire
    // P10  : Right
    // Set up the button pins as inputs with pull-ups
    GPIOB->CRL &=0xffffff0f;
    GPIOB->CRL |=0x00000080; 
    GPIOB->BSRR = BIT1;     
    GPIOB->CRH &=0xffff00ff;
    GPIOB->CRH |=0x00008800; 
    GPIOB->BSRR = BIT10+BIT11; 
    
    // Configure PC13 as an output
    GPIOC->CRH |= BIT20;
    GPIOC->CRH &= ~(BIT23 | BIT22 | BIT21);                
    
    // Power up the ADC
    RCC->APB2ENR |= BIT9; 
    // Set the prescaler for the ADC clock to divide by 8
    RCC->CFGR |= BIT15 + BIT14;
    // Turn on GPIO A
    RCC->APB2ENR |= BIT2;
    GPIOA->CRL &= 0xffffff00;   // configure pins for PA0 and PA1 as analog inputs
    // Will configure two ADC inputs.  One on PA0, the other PA1    
    ADC1->SMPR2 = 0x0000001d; // set sample times for channels 0 and 1 to 55.5 cycles
    ADC1->SQR1 = 0; // single sample in conversion sequence
    ADC1->SQR2 = 0; // single sample in conversion sequence
    ADC1->CR2 |= BIT0; // Turn on the ADC
    ADC1->CR2 |= BIT2; // enable calibration
    while ( ADC1->CR2 & BIT2 ); // wait for calibration to complete
    
    // rough default values
    TouchXMin = 300;
    TouchYMin = 570;
    TouchXMax = 3650;
    TouchYMax = 3720;
}
void controller::blink()
{
    GPIOC->ODR ^= BIT13;
}
uint16_t controller::getButtonState()
{
    uint16_t status=0;
    if ( (GPIOB->IDR & BIT1)==0)
        status = status | 1;
    if ( (GPIOB->IDR & BIT10)==0)
        status = status | 2;
    if ( (GPIOB->IDR & BIT11)==0)
        status = status | 4;
    return status;
}
uint16_t controller::readAnalogue(int ChannelNumber)
{
    uint16_t Result=0;
    //if (ChannelNumber == NoiseChannel)
    {
        ADC1->SQR3 = ChannelNumber; // Select channel number    
        ADC1->CR2 |= BIT0; // Trigger a conversion
        while ((ADC1->SR & BIT1)==0); // wait for end of conversion
        Result = (ADC1->DR & 0xffff); // return the result
    }
    return Result;    
}

uint16_t controller::readXTouch()
{
    // Returns a value in the range 0 to 1000 representing 0 to 100%
    uint32_t savedCRH = GPIOB->CRH;
    uint32_t savedCRL = GPIOB->CRL;
    uint32_t savedODR = GPIOB->ODR;
    GPIOB->BSRR = BIT6; // de-select the display
    // Configure PB5 as an output
    GPIOB->CRL &= ~(BIT23+BIT22+BIT21);
    GPIOB->CRL |= (BIT20);
    GPIOB->ODR &= ~BIT5; // zero PB5 (X-)
    // Configure PB13 as an output
    GPIOB->CRH &= ~(BIT23+BIT22+BIT21);
    GPIOB->CRH |= (BIT20);
    GPIOB->ODR |= BIT13; // set PB13 (X+)
    
    // Configure PB4 as an input 
    GPIOB->CRL &= ~(BIT19+BIT17+BIT16);
    GPIOB->CRL |= BIT18;
    
    // Configure PB12 as an input 
    GPIOB->CRH &= ~(BIT19+BIT17+BIT16);
    GPIOB->CRH |= BIT18;    
    
    uint32_t Vx = readAnalogue(1);
    // restore CRH,CRL
    GPIOB->CRH = savedCRH;
    GPIOB->CRL = savedCRL;    
    GPIOB->ODR = savedODR;
    Vx = ((TouchXMax - Vx) * 1000)/(TouchXMax - TouchXMin);
    if (Vx > 1000)
        Vx = 0xffff;
    return (Vx);
}
uint16_t controller::readYTouch()
{
    // Returns a value in the range 0 to 1000 representing 0 to 100%
    uint32_t savedCRH = GPIOB->CRH;
    uint32_t savedCRL = GPIOB->CRL;
    uint32_t savedODR = GPIOB->ODR;
    GPIOB->BSRR = BIT6; // de-select the display
    
    //Configure PB12 as an output
    GPIOB->CRH &= ~(BIT19+BIT18+BIT17);
    GPIOB->CRH |= (BIT16);
    GPIOB->ODR &= ~BIT12; // zero PB12 (Y-)
    
    // Configure PB4 as an output
    GPIOB->CRL &= ~(BIT19+BIT18+BIT17);
    GPIOB->CRL |= (BIT16);
    GPIOB->ODR |= BIT4; // set PB4 (Y+)
    
    // Configure PB13 as an input
    GPIOB->CRH &= ~(BIT23+BIT21+BIT20);
    GPIOB->CRH |= BIT22;
    // Configure PB5 as an input
    GPIOB->CRL &= ~(BIT23+BIT21+BIT20);
    GPIOB->CRL |= BIT22;    
    
    uint32_t Vy = readAnalogue(0);
    // restore CRH,CRL
    GPIOB->CRH = savedCRH;
    GPIOB->CRL = savedCRL;    
    GPIOB->ODR = savedODR;
    Vy = ((TouchYMax - Vy) * 1000)/(TouchYMax - TouchYMin);
    if (Vy > 1000) 
       Vy = 0xffff;
    return (Vy);  
    
}
void controller::calibrateTouch(uint16_t xmin,uint16_t xmax,uint16_t ymin,uint16_t ymax)
{
    TouchXMin = xmin;
    TouchYMin = ymin;
    TouchXMax = xmax;
    TouchYMax = ymax;
}
