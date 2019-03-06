#include <stdint.h>

class controller
{
public:
    controller(){};
    void begin();
    uint16_t getButtonState();    
    static const int NoiseChannel=16;
    uint16_t readAnalogue(int ChannelNumber);
    void blink(); // toggle the onboard LED 
    uint16_t readXTouch();
    uint16_t readYTouch();
    void calibrateTouch(uint16_t xmin,uint16_t xmax,uint16_t ymin,uint16_t ymax);
private:
    uint16_t TouchXMin;
    uint16_t TouchYMin;
    uint16_t TouchXMax;
    uint16_t TouchYMax;
};
