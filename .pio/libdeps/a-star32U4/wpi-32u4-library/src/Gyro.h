#pragma once

#include <LSM6.h>
#include <Wire.h>

class Gyro
{
public:
  Gyro();
  float getHeading();
  void loop();
  void reset();
  void setup();

private:
  LSM6 imu;
  float heading;
  float average;
  unsigned long gyroTimer;
  unsigned long lastTime;
};