#include "display.h"
#include "stm32f103.h"
void output_byte(uint8_t b);
void display::begin(timer &t)
{    
    // Low level I/O port initialisation
    // Turn on GPIO A and B
    RCC->APB2ENR |= BIT2 + BIT3;
    // Make all bits connected to the display outputs to start with
    // Note: PA15, PB4, PB3 are mapped to JTAG functionality at reset.    
    // Need to restore these to port operation first
    RCC->APB2ENR |= BIT0; // enable clock for AFIO latches
    AFIO->MAPR = 0x02000000; // turn off JTAG but leave SWD on  
    AFIO->MAPR |= BIT24; // free PB4 from JTAG duty
    // Port A bits 8,9,10,11,15    
    GPIOA->CRH = (GPIOA->CRH & 0x0fff0000) | 0x10001111;
    // Port B bits 3,4,5,6,12,13,14,15
    GPIOB->CRL = (GPIOB->CRL & 0xf0000fff) | 0x01111000;
    GPIOB->CRH = (GPIOB->CRH & 0x0000ffff) | 0x11110000;
    
    // get the display started
    // The set of steps carried out here are specific to the type of display being used
    
    
    
    RDHigh();
    CSHigh();
    WRHigh();
    RSTHigh();
    RSHigh();
    
    
    t.sleep(10);    
    RSTLow();
    t.sleep(10);
    RSTHigh();
    t.sleep(100);
    RDHigh();
    CSHigh();
    WRHigh();
    t.sleep(100);
    CSLow();
    // Power control startup sequence (from manufacturer)
    t.sleep(150);
	// Power control startup sequence
	writeCommandData(0x11, 0x2004);
	writeCommandData(0x13, 0xCC00);
	writeCommandData(0x15, 0x2600);
	writeCommandData(0x14, 0x252A);
	writeCommandData(0x12, 0x0033);
	writeCommandData(0x13, 0xCC04);
	writeCommandData(0x13, 0xCC06);
	writeCommandData(0x13, 0xCC4F);
	writeCommandData(0x13, 0x674F);
	writeCommandData(0x11, 0x2003);

	// Gamma control
	writeCommandData(0x30, 0x2609);
	writeCommandData(0x31, 0x242C);
	writeCommandData(0x32, 0x1F23);
	writeCommandData(0x33, 0x2425);
	writeCommandData(0x34, 0x2226);
	writeCommandData(0x35, 0x2523);
	writeCommandData(0x36, 0x1C1A);
	writeCommandData(0x37, 0x131D);
	writeCommandData(0x38, 0x0B11);
	writeCommandData(0x39, 0x1210);
	writeCommandData(0x3A, 0x1315);
	writeCommandData(0x3B, 0x3619);
	writeCommandData(0x3C, 0x0D00);
	writeCommandData(0x3D, 0x000D);

	writeCommandData(0x16, 0x0007); // power up
	writeCommandData(0x02, 0x0013); // LCD waveform control
	writeCommandData(0x03, 0x0003); // Interface mode
	writeCommandData(0x01, 0x0127); // driver output control
	writeCommandData(0x08, 0x0303); // Blank period
	writeCommandData(0x0A, 0x000B); // Frame cycle control
	writeCommandData(0x0B, 0x0003); // 
	writeCommandData(0x0C, 0x0000); // External interface control
	writeCommandData(0x41, 0x0000); // Vertical screen control
	writeCommandData(0x50, 0x0000); // MDDI
	writeCommandData(0x60, 0x0005); // MTP Init
	writeCommandData(0x70, 0x000B); // Timing
	writeCommandData(0x71, 0x0000); 
	writeCommandData(0x78, 0x0000);
	writeCommandData(0x7A, 0x0000);
	writeCommandData(0x79, 0x0007);
	writeCommandData(0x07, 0x0051); // Display control 
	writeCommandData(0x07, 0x0053);
	writeCommandData(0x79, 0x0000); // Panel signal control
    t.sleep(100);
    
    writeCommand((uint16_t) 0x22);    
    // black out the screen 
    fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x00);                    
}
void display::putPixel(uint16_t x, uint16_t y, uint16_t colour)
{
    openAperture(x, y, x + 1, y + 1);
    writeData(colour); 
}
void display::putImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *Image)
{
    uint16_t Colour;
    openAperture(x, y, x + width - 1, y + height - 1);
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            Colour = *(Image++);
            writeData(Colour);
        }
    }
}
void display::fillRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t Colour)
{
    openAperture(x, y, x + width - 1, y + height - 1);
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            writeData(Colour);
        }
    }
}
void display::resetDisplay()
{
    volatile int i=0; // don't optimize this variable out
    RSTLow();
    while(i < 20000) i++; // delay approx 12ms 
    RSTHigh();
    i=0;
    while(i < 20000) i++;
}
void display::writeCommand(uint8_t Cmd)
{
    RSLow();    
    output_byte(Cmd);
}
void display::writeCommand(uint16_t Cmd)
{    
    RSLow();    
    output_byte(Cmd >> 8);
    WRLow();
    WRHigh();
    output_byte(Cmd);
    WRLow();
    WRHigh();
}
void display::writeData(uint8_t Data)
{
    RSHigh();
    output_byte(Data);
}
void display::writeData(uint16_t Data)
{
    RSHigh();
    output_byte(Data >> 8);
    WRLow();
    WRHigh();
    output_byte(Data);
    WRLow();
    WRHigh();
}

void display::writeCommandData(uint16_t Cmd, uint16_t Data)
{
    writeCommand(Cmd);
    writeData(Data);
}
void display::openAperture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    // open up an area for drawing on the display
    
	writeCommandData(0x46, (x2 << 8) | x1);
	writeCommandData(0x47, y2);
	writeCommandData(0x48, y1);
	writeCommandData(0x20, x1);
	writeCommandData(0x21, y1);
	writeCommand((uint16_t)0x22);
	
	return;
    writeCommandData(0x36,x2); 
    writeCommandData(0x37,x1);
    writeCommandData(0x38,y2);
    writeCommandData(0x39,y1); 
    writeCommandData(0x20,x1);
    writeCommandData(0x21,y1); 	
    writeCommand((uint16_t)0x22);
}
inline void display::WRLow()
{
    GPIOB->BRR = BIT4; 
}
inline void display::WRHigh()
{
    GPIOB->BSRR = BIT4;
}
inline void display::CSLow()
{
    GPIOB->BRR = BIT6;
}
inline void display::CSHigh()
{
    GPIOB->BSRR = BIT6;
}
inline void display::RDLow()
{
    GPIOB->BRR = BIT3;
}
inline void display::RDHigh()
{
    GPIOB->BSRR = BIT3;
}
inline void display::RSTLow()
{
    GPIOA->BRR = BIT15;
}
inline void display::RSTHigh()
{
    GPIOA->BSRR = BIT15;
}
inline void display::RSLow()
{
    GPIOB->BRR = BIT5;
}
inline void display::RSHigh()
{
    GPIOB->BSRR = BIT5;        
}




/* The databus pins for the display are connected to the microcontroller pins
* in a mixed up order because it simplifies construction.  This mixing of the 
* bit order needs to be fixed up in software.  This function handles this
* // Bit reversal lookup table (16 entries or 4 bits)
* 
* /*
* bit   rev
* 0000  0000
* 0001  1000
* 0010  0100
* 0011  1100
* 0100  0010
* 0101  1010
* 0110  0110
* 0111  1110
* 1000  0001
* 1001  1001
* 1010  0101
* 1011  1101
* 1100  0011
* 1101  1011
* 1110  0111
* 1111  1111
*/
const uint8_t reverse_bits[]={0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15 };

void output_byte(uint8_t b)
{
    /*
    * DB0 : A11
    * DB1 : A10
    * DB2 : A9
    * DB3 : A8
    * DB4 : B15
    * DB5 : B14
    * DB6 : B13
    * DB7 : B12
    */
    
    GPIOA->BRR = BIT8+BIT9+BIT10+BIT11;
    GPIOB->BRR = BIT12+BIT13+BIT14+BIT15;    
    if (b)
    { // only write out bits if b is non zero to save time
        GPIOA->BSRR = ((uint32_t)reverse_bits[b & 0x0f]) << 8;
        GPIOB->BSRR = ((uint32_t)reverse_bits[b >> 4]) << 12;        
    }
}




 
