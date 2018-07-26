/* This program exercises the debug object
 */

#include "console.h"
#include "sprite.h"
#include "brici.h"
#include "invaders.h"
#include "spritedesigner.h"
#define BGND COLOUR(0,0,0)
#define IVDR COLOUR(0xff,0xff,0x00)

void OnTimer()
{
    static int Count=0;
    Count++;
    if (Count > 1000)
    {
        Console.Controller.blink();
        Count = 0;
    }
    Console.random(0,1);
    
}
void setup()
{
    Console.Timer.attach(OnTimer);    
}
int x;
int y=90;
#define STARTUP_MELODY_NOTES 10
const uint32_t StartupMelodyTones[STARTUP_MELODY_NOTES]={1000,500,1000,500,1000,500,1000,500,1000,500};
const uint32_t StartupMelodyTimes[STARTUP_MELODY_NOTES]={200,200,200,200,200,200,200,200,200,200};
void loop()
{    
    
    int GameStarted = 0;
    Console.fillRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    Console.print("Bread", 5, SCREEN_WIDTH/3, 50, RGBToWord(0xff, 0xff, 0x0), 0);
    Console.print("Board", 5, SCREEN_WIDTH/3, 70, RGBToWord(0xff, 0xff, 0x0), 0);
    Console.print("Games!", 5, SCREEN_WIDTH/3, 90, RGBToWord(0xff, 0xff, 0x0), 0);
    Console.print("Left for Brici", 14, 5, 120, RGBToWord(0xff, 0x3f, 0x3f), 0);
    Console.print("Right for Galaga", 16, 5, 140, RGBToWord(0x1f, 0xff, 0x1f), 0);    
    Console.print("Fire for Sprite design", 22, 5, 160, RGBToWord(0x1f, 0xff, 0x1f), 0);    
    Console.print("web:ioprog.com/bbg", 18, 5, 200, RGBToWord(0xff, 0xff, 0xff), 0);
    Console.Sound.playMelody(StartupMelodyTones, StartupMelodyTimes,STARTUP_MELODY_NOTES);
    while (GameStarted == 0)
    {
        if (Console.Controller.getButtonState()==1)
        {
            GameStarted = 1;
            playBrici();
        }
        if (Console.Controller.getButtonState()==2)
        {
            GameStarted = 1;
            playInvaders();      
        }        
        if (Console.Controller.getButtonState()==4)
        {
            GameStarted = 1;
            SpriteDesigner();      
        }
        Console.Timer.sleep(100);
    }
    
}



