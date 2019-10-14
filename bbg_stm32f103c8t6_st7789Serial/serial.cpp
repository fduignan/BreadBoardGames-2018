#include "serial.h"
#include "stm32f103.h"
static serial *pSerial;  // need a pointer to the hopefully only instance of Serial for the interrupt service routine
void serial::begin()
{
    int BaudRateDivisor;
    int BaudRate = 9600;
    pSerial = this;
	disable_interrupts();
	RXBuffer.head = RXBuffer.tail = RXBuffer.count = 0;
	TXBuffer.head = TXBuffer.tail = TXBuffer.count = 0;	
// Turn on the clock for GPIOA (usart 2 uses it)
	RCC->APB2ENR  |= BIT2;
// Turn on the clock for the USART2 peripheral	
	RCC->APB1ENR |= BIT17;

	
// Configure the I/O pins.  Will use PA3 as RX and PA2 as TX
// This is the default alternative function for PA2 and PA3
    // PA2, TX : make it an output first    
    GPIOA->CRL &= ~(BIT8);
    GPIOA->CRL |= (BIT9);
    // PA2, TX : Turn on alternate function
    GPIOA->CRL &= ~(BIT10);
    GPIOA->CRL |= (BIT11);
    
    // PA3, RX : make it an input
    GPIOA->CRL &= ~(BIT13 + BIT12);
    // PA3, RX : Make it a floating input (maybe should use pull-up?)
    GPIOA->CRL &= ~(BIT14);
    GPIOA->CRL |= (BIT15);	   
	
	BaudRateDivisor = 72000000; // assuming 72MHz APB1 clock (this is wrong but seems to work. PCLK1 *should* be 36MHz)
	BaudRateDivisor = BaudRateDivisor / (long) BaudRate;

// For details of what follows refer to RM008, USART registers.

// De-assert reset of USART2 
	RCC->APB1RSTR &= ~BIT17;
// Configure the USART
// disable USART first to allow setting of other control bits
// This also disables parity checking and enables 16 times oversampling

	USART2->CR1 = 0; 
 
// Don't want anything from CR2
	USART2->CR2 = 0;

// Don't want anything from CR3
	USART2->CR3 = 0;

// Set the baud rate    
    
    /* Divisor = 36000000/(16*9600)=234.375
     * The DIV Fractional part is 0.375 * 16 = 6
     * The Mantissa = 234.  The BRR is divided as follows 
     * Bits15:Bit4 = Mantissa
     * Bit3:Bit0 = Fractional part
     * These are combined as follows:
     * Mantissa << 4 + Fractional part
     * This is equivalent to dividing 36000000 by the desired baud rate
    */
	USART2->BRR = BaudRateDivisor;

// Turn on Transmitter, Receiver, Transmit and Receive interrupts.
	USART2->CR1 |= (BIT2  | BIT3 |  BIT5 | BIT6 ); 
// Enable the USART
	USART2->CR1 |= BIT13;
// Enable USART2 interrupts in NVIC	
	NVIC->ISER1 |= BIT6; // enable interrupt # 38 (USART2) Bit number = 38 - 32
    enable_interrupts();
}
void serial::eputc(char c)
{
	if ( (USART2->CR1 & BIT3)==0)
	{ // transmitter was idle, turn it on and force out first character
	  USART2->CR1 |= BIT3;
	  USART2->DR = getBuf(TXBuffer);		
	} 
    else
    {   // interrupt transmission is underway so add byte to the queue
        putBuf(TXBuffer,c);
    }
}
char serial::egetc()
{
    // return next character in buffer (0 if empty)
    return getBuf(RXBuffer);
}
void serial::print(int Value)
{
    // Convert Value to a string equivalent and call the 
    // print(string) version of this function
    
    char Buffer[12]; // ints can be +/- 2 billion (approx) so buffer needs to be this big
    int index=10;    // Start filling buffer from the right
    Buffer[11]=0;    // ensure the buffer is terminated with a null
    if (Value < 0)  
    {
        Buffer[0]='-';  // insert a leading minus sign
        Value = -Value; // make Value positive
    }
    else
    {
        Buffer[0]='+';  // insert a leading plus sign
    }
    while(index > 0)  // go through all of the characters in the buffer
    {
        Buffer[index]=(Value % 10)+'0'; // convert the number to ASCII
        Value = Value / 10; // move on to the next digit
        index--;  
    }
    print(Buffer); // call string version of print
}
void serial::print(const char *String)
{
    while(*String) // keep going until a null is encountered
    {
        eputc(*String); // write out the next character
        String++;       // move the pointer along
    }
}
// Maybe the circular buffer code belongs in a more general purpose module
void serial::putBuf(SerialBuffer &sbuf, char c)
{
    if (sbuf.count < SBUFSIZE)
    {
        disable_interrupts();
        sbuf.count++;
        sbuf.buffer[sbuf.head] = c;
        sbuf.head=(sbuf.head+1) % SBUFSIZE;
        enable_interrupts();
    }
}
char serial::getBuf(SerialBuffer &sbuf)
{
    char c=0;
    if (sbuf.count >0 )
    {
        disable_interrupts();
        sbuf.count--;
        c=sbuf.buffer[sbuf.tail];
        sbuf.tail=(sbuf.tail+1) % SBUFSIZE;
        enable_interrupts();
    }
    return c;
}
// This is an interrupt handler for serial communications.  
void USART2_Handler(void)
{
// check which interrupt happened.       
    if (USART2->SR & BIT6) // is it a TXE interrupt?
    {
        if (pSerial->TXBuffer.count > 0) // if there is another byte to send
        {
            USART2->DR=pSerial->getBuf(pSerial->TXBuffer);
        }
        else
        {
            // No more data, disable the transmitter 
            USART2->CR1 &= ~BIT3;
            if (USART2->SR & BIT6)
            // Clear the TC interrupt flag
            USART2->SR &= ~BIT6;	  
        }
    }
	if (USART2->SR & BIT5) // is it an RXNE interrupt?
    {
        // read the RX hardware buffer and put 
        // it into the circular buffer
        pSerial->putBuf(pSerial->RXBuffer,USART2->DR);         	
    }

}
