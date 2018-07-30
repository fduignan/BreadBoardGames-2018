// Jingle designer 
#include "console.h"

void JingleDesigner()
{
#define GRID_HEIGHT 16
#define GRID_WIDTH 14
    
    int row,col;
    int cellsize;
    cellsize = SCREEN_WIDTH / GRID_WIDTH;
    int Done=0;
    Console.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);   // black out the screen
    const char NoteString[][3]={"C ", "C " , "D " , "D " , "E " , "F " , "F ", "G " , "G " , "A " , "B " , "B " , "O " , "O "};    
    
    for (col = 0; col < GRID_WIDTH; col++)
    {
         Console.print(&NoteString[col][0],2,col*cellsize+2,0,RGBToWord(0xff,0xff,0xff),0);
    }
// Need to print the #'s and b's separately to pack them on screen properly
    Console.print("#",1,1*cellsize+5+2,0,RGBToWord(0xff,0xff,0xff),0);
    Console.print("#",1,3*cellsize+5+2,0,RGBToWord(0xff,0xff,0xff),0);
    Console.print("#",1,6*cellsize+5+2,0,RGBToWord(0xff,0xff,0xff),0);
    Console.print("#",1,8*cellsize+5+2,0,RGBToWord(0xff,0xff,0xff),0);
    Console.print("b",1,10*cellsize+5+2,0,RGBToWord(0xff,0xff,0xff),0);
    for (row = 0; row < GRID_HEIGHT ; row ++)
    {
        for (col = 0; col < GRID_WIDTH; col++)
        {
           
            Console.drawRectangle(col*cellsize,10+row*cellsize,cellsize,cellsize,RGBToWord(0xff,0xff,0xff));            
        }
    } 
    Console.print("Cancel Save Restore",sizeof("Cancel Save Restore")-1,0,SCREEN_HEIGHT-8,RGBToWord(0xff,0xff,0xff),0);
    while(!Done)
    {
        uint32_t Vx = (Console.Controller.readXTouch() * SCREEN_WIDTH)/1000;
        uint32_t Vy = (Console.Controller.readYTouch() * SCREEN_HEIGHT)/1000;
        Console.Serial.print(Vx);
        Console.Serial.print(" ");
        Console.Serial.print(Vy);
        Console.Serial.print("\r\n");
        if ( (Vx < SCREEN_WIDTH) && (Vy < SCREEN_HEIGHT) )
        {
            row = (Vy / cellsize);
            col = (Vx / cellsize);
            if ( (row < GRID_HEIGHT+1) && (col < GRID_WIDTH) )
            {
                Console.fillRectangle((col*cellsize)+1,(row*cellsize)-1,cellsize-1,cellsize-1,RGBToWord(0xff,0,0x00));
            }
        }
        int Buttons = Console.Controller.getButtonState(); // wait for the user to push a button 
        Console.Timer.sleep(50);         
        switch (Buttons)
        {
            case 1 : {
                // Cancel
                Done=1;
                break;
            }
            case 2 : {
                // restore original              
                Done=1;
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
