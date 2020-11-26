/*
 * A minimal class to control a servo on pin 5.
 * 
 * It uses output compare on Timer3 to control the pulse. The 16-bit Timer3 is set up (in Init()) 
 * with a pre-scaler of 8, TOP of 39999 + 1 => 20 ms
 * 
 * OCR3A controls the pulse on pin 5 -- THE SERVO MUST BE ON PIN 5! 
 * 
 * Defaults to a range of 1000 - 2000 us, but can be customized.
 */ 

#include <Arduino.h>

class Servo32U4
{
private:
    uint16_t usMin = 1000;
    uint16_t usMax = 2000;

    uint8_t feedbackPin = -1;
public:
    static void Init(void);
    static void Attach(void); //MUST USE PIN 5
    void Detach(void);
    void Write(uint16_t microseconds);
    uint16_t SetMinMaxUS(uint16_t min, uint16_t max);
};
