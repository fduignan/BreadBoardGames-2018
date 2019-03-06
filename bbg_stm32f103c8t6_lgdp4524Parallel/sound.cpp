#include "sound.h"
#include <stdint.h>
#include "stm32f103.h"
static sound *pSound; // A hack to to allow registration of callbacks while decoupling the sound and timer classes.

void sound::begin()
{
    pSound = this;
     // Output is a piezo speaker on B0 which is Timer 3, Channel 3
    // Turn on Timer 3
    RCC->APB1ENR |= BIT1; 
    // Ensure that Port B is enabled
    RCC->APB2ENR |= BIT3;
    // Set Timer 3 to default values
    TIM3->CR1 = 0; 
    // Enable PWM mode on channel 3
    TIM3->CCMR2_Output = BIT6+BIT5; 
    // Connect up Timer 3 output
    TIM3->CCER |= BIT8;
}
void sound::playTone(uint16_t Frequency, uint16_t ms)
{
 // Configure PB0 as Timer3 output
    GPIOB->CRL &=0xfffffff0;
    GPIOB->CRL |= BIT3+BIT1;
    // Will assume a 72MHz input frequency
    // The auto-reload register has a maximum value of 65536.  
    // This should map to the lowest frequency we would be interested in.
    // 72MHz/65536 = 1098Hz.  This is too high for a 'lowest' frequency so 
    // some pre-scaling of the input frequency is required.
    TIM3->CR1 &= ~BIT0; // disable the timer    
    TIM3->PSC = 72000000UL/65536UL;  // prescale so that the lowest frequency is 1Hz.
    TIM3->ARR = (72000000UL/(uint32_t)(TIM3->PSC))/((uint32_t)Frequency);
    TIM3->CCR3 = TIM3->ARR/2; // 50 % Duty cycle
    TIM3->CNT = 0;
    TIM3->CR1 |= BIT0; // enable the timer        
    tone_time = ms;    
}
void sound::stopTone()
{
    TIM3->CR1 &= ~BIT0; // disable the timer
    // Force the output low
     // Configure PB0 as a high impedance input (seems to save power)
    GPIOB->CRL &=0xfffffff0;
    GPIOB->CRL |= 4;
	
}
int  sound::SoundPlaying()
{
    if (tone_time > 0)
        return 1;
    else        
        return 0;
}
void sound::playMelody(const uint16_t *Tones,const uint16_t *Times,int Len)
{
    int Index;
    for (Index = 0; Index < Len; Index++)
    {
        while(SoundPlaying()); // wait for previous note to complete        
        playTone(Tones[Index],Times[Index]);               
    }
}
void sound::ms_callback()
{
    // This callback should run every millisecond.  It is attached to the Timer object in console.cpp:begin
    if (pSound->tone_time)
    {
        pSound->tone_time--;
        if (pSound->tone_time == 0)
        {
            pSound->stopTone();
        }
    }
}
