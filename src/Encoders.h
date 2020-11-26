#ifndef ENCODER
#define ENCODER

#include <Romi32U4.h>

class Encoder{
    private:
        const float N_wheel = 1440.0; //counts per wheel revolution
        const float R_wheel = 35.0; //radius of wheel in [mm]
        const float C_wheel = 2*PI*R_wheel; //circumference of wheel
        const int interval = 50; // time in [ms], how often encoders are being updated
        
    public:
        void Init(void);
        int ReadEncoderCountLeft(void);
        int ReadEncoderCountRight(void);
        float ReadVelocityLeft(void); 
        float ReadVelocityRight(void); 
        float PrintVelocities(void);
        boolean UpdateEncoderCounts(void);
};

#endif