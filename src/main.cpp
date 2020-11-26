#include <Arduino.h>
#include "Behaviors.h"

Behaviors followWaypoints;

void setup() {
  followWaypoints.Init();
}

void loop() {
  followWaypoints.Run();
}