#include "console.h"
#include "invaders.h"
#define SPRITE_WIDTH 10
#define SPRITE_HEIGHT 12

void CalibrateTouchScreen()
{
    Console.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGBToWord(0,0,0));  
    Console.drawRectangle(0,0,SCREEN_WIDTH, SCREEN_HEIGHT,RGBToWord(0xff,0xff,0xff));    

}

void SpriteDesigner(void)
{
    // This editor makes use of the touch screen (in the Controller object)
    // 
    int row,col;
    int SelectedRow,  SelectedCol, SelectedPixel,PreviousSelectedPixel;
    int SpriteWidth, SpriteHeight;   
    uint16_t BrushColour;
    int cellsize;
    uint16_t *Image=Console.Storage.nvm_data.InvaderSprite;// sprite pixel buffer        
    int PixelIndex = 0;
    int ColourIndex = 0;
    #define PALLETTE_ROW_COUNT 2
    #define PALLETTE_COL_COUNT 11
    uint16_t Colour[]={ RGBToWord(0,0,0), RGBToWord(0x80,0x80,0x80), RGBToWord(0xff,0,0), RGBToWord(0xff,0x80,0), RGBToWord(0xff,0xff,0), RGBToWord(0,0xff,0), RGBToWord(0,0xff,0xff), RGBToWord(0,0,0xff),RGBToWord(0xff,0,0xff),  RGBToWord(0xff,0x80,0x80), RGBToWord(0x80,0xff,0x80), RGBToWord(0xff,0xff,0xff), RGBToWord(0xc0,0xc0,0xc0), RGBToWord(0x80,0,0), RGBToWord(0x80,0x40,0), RGBToWord(0x80,0x80,0), RGBToWord(0,0x80,0), RGBToWord(0,0x80,0x80), RGBToWord(0,0,0x80), RGBToWord(0x80,0,0x80), RGBToWord(0x80,0x80,0xff), RGBToWord(0xff,0xff,0xc0) };
    int Buttons = 0;
    cellsize=(SCREEN_WIDTH * 8 / 10) / SPRITE_WIDTH;
    int Done=0;    
    int PalletteYOffset = (cellsize*SPRITE_HEIGHT)+4;
    
    // Make a copy of the sprite in RAM first
    if (!Console.Storage.DataPresent())
    {
        while (PixelIndex < (sizeof(InvaderImage)/sizeof(uint16_t)))
        {
            Image[PixelIndex]=InvaderImage[PixelIndex];        
            PixelIndex++;
        }
    }
    PixelIndex = 0;
    Console.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);   // black out the screen
    // Draw a magnified version of the sprite
    for (row = 0; row < SPRITE_HEIGHT; row ++)
    {
        for (col = 0; col < SPRITE_WIDTH; col++)
        {
            Console.drawRectangle(col*cellsize,row*cellsize,cellsize,cellsize,RGBToWord(0xff,0xff,0xff));
            Console.fillRectangle((col*cellsize)+1,(row*cellsize)+1,cellsize-2,cellsize-2,Image[PixelIndex]);
            PixelIndex++;
        }
    } 
    // Draw the colour pallette
    for (row = 0; row < PALLETTE_ROW_COUNT; row ++)
    {
        for (col = 0; col < PALLETTE_COL_COUNT; col++)
        {
            Console.drawRectangle(col*cellsize,row*cellsize+PalletteYOffset,cellsize,cellsize,RGBToWord(0xff,0xff,0xff));
            Console.fillRectangle((col*cellsize)+1,(row*cellsize)+PalletteYOffset+1,cellsize-2,cellsize-2,Colour[ColourIndex]);
            ColourIndex++;
        }
    } 
    Console.drawRectangle(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, RGBToWord(0xff,0xff,0));
    ColourIndex = 0;
    while (!Done)
    {             
        
        uint32_t Vx = (Console.Controller.readXTouch() * SCREEN_WIDTH)/1000;
        uint32_t Vy = (Console.Controller.readYTouch() * SCREEN_HEIGHT)/1000;
        Console.Serial.print(Vx);
        Console.Serial.print(" ");
        Console.Serial.print(Vy);
        Console.Serial.print("\r\n");
        if ( (Vx < SCREEN_WIDTH) && (Vy < SCREEN_HEIGHT) )
        {            
            
            //Console.fillRectangle(Vx,Vy,2,2,RGBToWord(0xff,0,0));            
            if ( (Vy >= PalletteYOffset) && ( Vy <= PalletteYOffset + 2*cellsize)  && (Vx < 11 * cellsize))                 
            {
                // in colour pallette region
                ColourIndex = ((Vy - PalletteYOffset)/cellsize)*PALLETTE_COL_COUNT+(Vx/cellsize);
                Console.Serial.print(ColourIndex);
            }
            Console.Serial.print("\r\n");       
            Console.drawRectangle(SPRITE_WIDTH*cellsize+10,SCREEN_HEIGHT/2,cellsize,cellsize,RGBToWord(0xff,0xff,0xff));
            Console.fillRectangle(SPRITE_WIDTH*cellsize+11,(SCREEN_HEIGHT/2)+1,cellsize-2,cellsize-2,Colour[ColourIndex]);
            if ( (Vy <= SPRITE_HEIGHT*cellsize) && ( Vx <= (SPRITE_WIDTH * cellsize)) )
            {
                // in sprite region
                row = (Vy / cellsize);
                col = (Vx / cellsize);
                PixelIndex = row * SPRITE_WIDTH + col;
                //if (PixelIndex < (SPRITE_WIDTH * SPRITE_HEIGHT))
                if ( (row < SPRITE_HEIGHT) && (col < SPRITE_WIDTH) )
                {
                    // update image
                    Image[PixelIndex] = Colour[ColourIndex];                    
                    // update magnified image
                    Console.fillRectangle((col*cellsize)+1,(row*cellsize)+1,cellsize-2,cellsize-2,Image[PixelIndex]);
                    // update preview
                    Console.putImage(SPRITE_WIDTH*cellsize+11, (SCREEN_HEIGHT/2) - 2*cellsize, SPRITE_WIDTH, SPRITE_HEIGHT, Image);
                }
            }
            
            
        }            
        Buttons = Console.Controller.getButtonState(); // wait for the user to push a button 
        Console.Timer.sleep(50);         
        switch (Buttons)
        {
            case 1 : {
                // Cancel
                return;
                break;
            }
            case 4 : {
                // OK (Save)
                Console.Storage.writeData();
                Done=1;
                break;
            }               
        }                                                                     
    }    
}
