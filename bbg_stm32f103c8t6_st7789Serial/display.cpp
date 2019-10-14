#include "display.h"
#include "stm32f103.h"
#include "spi.h"
// i/o list
// A9 : D/C pin 
// A8 : Reset pin 
// B15 : SDA
// B13 : SCK

void output_byte(uint8_t b);
void display::begin(timer &t)
{    
    // Get the SPI interface going for the ST7789
    initSPI();
    // Low level I/O port initialisation
    // Turn on GPIO A 
    RCC->APB2ENR |= BIT2;
    // Port A bits 8,9
    GPIOA->CRH = (GPIOA->CRH & 0xffffff00) | 0x00000011;
    // get the display started
    // The set of steps carried out here are specific to the type of display being used
    
    
    while(0)
    {
        t.sleep(1);
        transferSPI16(0x1166);
    }
    RSTHigh(); // Drive reset high
    t.sleep(25); // wait   
    RSTLow(); // Drive reset low
    t.sleep(25); // wait   
    RSTHigh(); // Drive reset high
    t.sleep(25); // wait    
    
    
    writeCommand(0x1);  // software reset
    t.sleep(150); // wait   
    
    writeCommand(0x11);  //exit SLEEP mode
    t.sleep(25); // wait   
    
    writeCommand(0x3A); // Set colour mode        
    writeData8(0x55); // 16bits / pixel @ 64k colors 5-6-5 format 
    t.sleep(25); // wait   
    
    writeCommand(0x36);
    writeData8(0x08);  // RGB Format
    t.sleep(25); // wait   
    
    
    writeCommand(0x51); // maximum brightness
    t.sleep(25); // wait   
    
    writeCommand(0x21);    // display inversion off (datasheet is incorrect on this point)
    writeCommand(0x13);    // partial mode off                 
    writeCommand(0x29);    // display on
    t.sleep(25); // wait   
    writeCommand(0x2c);   // put display in to write mode
    fillRectangle(0,0,SCREEN_WIDTH, SCREEN_HEIGHT, 0x0000);  // black out the screen  
    
    
}
void display::putPixel(uint16_t x, uint16_t y, uint16_t colour)
{
    openAperture(x, y, x + 1, y + 1);
    writeData16(colour); 
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
            writeData16(Colour);
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
            writeData16(Colour);
        }
    }
}
void display::resetDisplay()
{
    volatile int i=0; // don't optimize this variable out
    RSTLow();
    while(i < 80000) i++; // delay approx 12ms 
    RSTHigh();
    i=0;
    while(i < 80000) i++;
}
void display::writeCommand(uint8_t Cmd)
{
    DCLow();    
    transferSPI8(Cmd);
}

void display::writeData8(uint8_t Data)
{
    DCHigh();
    transferSPI8(Data);
}
void display::writeData16(uint16_t Data)
{
    DCHigh();
    transferSPI16(Data);
}
void display::openAperture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
// open up an area for drawing on the display
    writeCommand(0x2A); // Set X limits
    writeData8(x1>>8);
    writeData8(x1&0xff);    
    writeData8(x2>>8);
    writeData8(x2&0xff);
    writeCommand(0x2B); // Set Y limits
    writeData8(y1>>8);
    writeData8(y1&0xff);    
    writeData8(y2>>8);
    writeData8(y2&0xff);
    
    writeCommand(0x2c); // put display in to data write mode
}


inline void display::RSTLow()
{
    GPIOA->BRR = BIT8;
}
inline void display::RSTHigh()
{
    GPIOA->BSRR = BIT8;
}
inline void display::DCLow()
{
    GPIOA->BRR = BIT9;
}
inline void display::DCHigh()
{
    GPIOA->BSRR = BIT9;        
}

