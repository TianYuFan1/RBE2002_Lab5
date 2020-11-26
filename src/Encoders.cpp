#include <Romi32U4.h>
#include "Encoders.h"

int count_left = 0;
int count_right = 0;
int prev_count_left = 0;
int prev_count_right = 0;
float previous_time = 0;
uint32_t lastUpdate = 0;

Romi32U4Encoders encoders;

void Encoder::Init(void)
{
    //nothing to initialize, however, good practice to have a init function anyway
}

float Encoder::PrintVelocities(void)
{
    Serial.print("Velocity of left wheel: ");
    Serial.print(ReadVelocityLeft());
    Serial.print('\t');
    Serial.print("Velocity of right wheel: ");
    Serial.println(ReadVelocityRight());
}   

int Encoder::ReadEncoderCountLeft(void)
{
  return count_left;
}

int Encoder::ReadEncoderCountRight(void)
{
  return count_right;
}

float Encoder::ReadVelocityLeft(void)
{
    float measurement = (C_wheel/N_wheel)*(count_left-prev_count_left)/((float)interval/1000);
    return measurement;
}

float Encoder::ReadVelocityRight(void)
{
    float measurement = (C_wheel/N_wheel)*(count_right-prev_count_right)/((float)interval/1000);
    return measurement;
}

boolean Encoder::UpdateEncoderCounts(void)
{
  uint32_t now = millis();
  if(now - lastUpdate >= interval)
  {    
    prev_count_left = count_left;
    prev_count_right = count_right;
    count_left = encoders.getCountsLeft();
    count_right = encoders.getCountsRight();
    previous_time = millis();
    lastUpdate = now;
    return 1;
  }
  return 0;
}