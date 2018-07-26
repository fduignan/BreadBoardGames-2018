#ifndef __sound_h
#define __sound_h
#include <stdint.h>
class sound
{
public:
    sound(){};
    void begin();
    void playTone(uint32_t Frequency, uint32_t ms);
    static void stopTone();
    int  SoundPlaying();
    void playMelody(const uint32_t *Tones,const uint32_t *Times,int Len);
    static void ms_callback(void);
private:
    uint32_t tone_time;          
};
#endif
