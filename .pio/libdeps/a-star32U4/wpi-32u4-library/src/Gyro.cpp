#include "Gyro.h"

Gyro::Gyro() {}

void Gyro::setup()
{
  Wire.begin();

  if (!imu.init())
  {
    // Failed to detect the LSM6.
    while (1)
    {
      Serial.println(F("Failed to detect the LSM6."));
      delay(100);
    }
  }
  imu.enableDefault();
  imu.setFullScaleGyro(LSM6::GYRO_FS1000);
  imu.setGyroDataOutputRate(LSM6::GYRO_ODR104);

  // Sample the z velocities
  long sum = 0;
  int samples = 2000;
  for (int i = 0; i < samples; i++)
  {
    while ((imu.getStatus() & 0x02) == 0)
      ;
    imu.readGyro();
    sum += imu.g.z;
  }
  average = sum / samples;
  imu.setGyroDataOutputRate(LSM6::GYRO_ODR52);

  reset();
}

void Gyro::reset()
{
  heading = 0.0;
  gyroTimer = millis() + 10;
  lastTime = 0;
  Serial.print("Average: ");
  Serial.println(average);
}

float Gyro::getHeading() { return heading; }

void Gyro::loop()
{
  if (imu.getStatus() & 0x02)
  {
    imu.readGyro();
    heading += (imu.g.z - average) * 0.035 / 104;
  }
}
