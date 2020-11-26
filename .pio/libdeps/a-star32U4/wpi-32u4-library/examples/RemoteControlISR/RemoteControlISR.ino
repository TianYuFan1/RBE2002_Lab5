
#include <Romi32U4.h>
#include "RemoteConstants.h"
#include "IRdecoder.h"

Romi32U4Motors motors;

IRDecoder decoder;

void setup()
{
  decoder.init();
}

void loop()
{ 
  delay(1);
  int32_t keyCode = decoder.getKeyCode();

  if(keyCode >= 0)
  {
    Serial.print(keyCode); 
    // Serial.print('\t');
    // Serial.print((uint8_t)(datum >> 16));
    Serial.print('\n');

    static int16_t left = 0;
    static int16_t right = 0;

    if(keyCode == 90) {left += 5; right += 5;}
    if(keyCode == 82) {left -= 5; right -= 5;}
    if(keyCode == 76) {left -= 5; right += 5;}
    if(keyCode == 81) {left += 5; right -= 5;}
    if(keyCode == 92) //slow down FAST, but not emergency stop
    {
      if(abs(left) <= 40) left = 0;
      else if(left > 0) left -= 25;
      else left += 25;

      if(abs(right) <= 40) right = 0;
      else if(right > 0) right -= 25;
      else right += 25;
    }

    if(keyCode == 72) {int16_t avg = (left + right) / 2; left = avg; right = avg;} //drive straight

    motors.setEfforts(left, right);
  }
}
