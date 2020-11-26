// Copyright Pololu Corporation.  For more information, see http://www.pololu.com/

#include <Romi32U4Motors.h>
#include <FastGPIO.h>
#include <avr/io.h>

#define PWM_L 10
#define PWM_R 9
#define DIR_L 16
#define DIR_R 15

bool Romi32U4Motors::flipLeft = false;
bool Romi32U4Motors::flipRight = false;

int16_t Romi32U4Motors::maxEffort = 300;

// initialize timer1 to generate the proper PWM outputs to the motor drivers
void Romi32U4Motors::init2()
{
    FastGPIO::Pin<PWM_L>::setOutputLow();
    FastGPIO::Pin<PWM_R>::setOutputLow();
    FastGPIO::Pin<DIR_L>::setOutputLow();
    FastGPIO::Pin<DIR_R>::setOutputLow();

    // Timer 1 configuration
    // prescaler: clockI/O / 1
    // outputs enabled
    // phase-correct PWM
    // top of 400
    //
    // PWM frequency calculation
    // 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
    TCCR1A = 0xA8; //0b10101000; //gcl: added OCR1C for adding a third PWM on pin 11
    TCCR1B = 0x11; //0b00010001;
    ICR1 = 400;

    //set all three outputs to zero
    OCR1A = 0;
    OCR1B = 0;
    OCR1C = 0;
}

void Romi32U4Motors::flipLeftMotor(bool flip)
{
    flipLeft = flip;
}

void Romi32U4Motors::flipRightMotor(bool flip)
{
    flipRight = flip;
}

void Romi32U4Motors::setLeftEffort(int16_t effort)
{
    init();

    bool reverse = 0;

    if (effort < 0)
    {
        effort = -effort; // Make speed a positive quantity.
        reverse = 1;    // Preserve the direction.
    }
    if (effort > maxEffort)
    {
        effort = maxEffort;
    }

    OCR1B = effort;

    FastGPIO::Pin<DIR_L>::setOutput(reverse ^ flipLeft);
}

void Romi32U4Motors::setRightEffort(int16_t effort)
{
    init();

    bool reverse = 0;

    if (effort < 0)
    {
        effort = -effort;  // Make speed a positive quantity.
        reverse = 1;     // Preserve the direction.
    }
    if (effort > maxEffort)
    {
        effort = maxEffort;
    }

    OCR1A = effort;

    FastGPIO::Pin<DIR_R>::setOutput(reverse ^ flipRight);
}

void Romi32U4Motors::setEfforts(int16_t leftEffort, int16_t rightEffort)
{
    setLeftEffort(leftEffort);
    setRightEffort(rightEffort);
}

void Romi32U4Motors::allowTurbo(bool turbo)
{
    maxEffort = turbo ? 400 : 300;
}
