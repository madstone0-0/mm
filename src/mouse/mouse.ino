#include <ArduinoSTL.h>
#include "FloodMouse.hpp"
#include "Mouse_API.hpp"

Algorithm* setupAlgo() {
  Mouse_API api{};
delay(100);
  FloodMouse flood{&api, {16,16}};
delay(100);
  Algorithm* algo = &flood;
return algo;
}
Algorithm* algo = nullptr;
void setup() {
  algo = setupAlgo();
  Serial.begin(9600);
  while(!Serial){}
  Serial.println(__cplusplus);
    pinMode(13, OUTPUT);
}

void loop() {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
}
