
#include <ArduinoSTL.h>
#include "FloodMouse.hpp"
#include "Mouse_API.hpp"

void blink(int pin, int d) {
  digitalWrite(pin, HIGH);
  delay(d);
  digitalWrite(pin, LOW);
  delay(d);
}

// AlgoHandler handler{};
// Algorithm* algo{ handler.algo };
using Pins = std::array<uint8_t, 4>;
Pins leftMotor{2, 5, 3, 4};
Pins rightMotor{6, 7, 8, 9};
Mouse_API mouse{leftMotor, rightMotor};
FloodMouse flood{ &mouse, Coords{ 16, 16 }, vector<Coords>{ { 7, 7 }, { 7, 8 }, { 8, 7 }, { 8, 8 } } };

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  pinMode(13, OUTPUT);
  for(const auto& i : leftMotor) pinMode(i, OUTPUT);
  for(const auto& i : rightMotor) pinMode(i, OUTPUT);
}


void loop() {
  Serial.println("Here");
  delay(100);
  // blink(13, 100);
  Serial.println("Beginning algo");
  flood.start();
  Serial.println("Ending algo");
  // blink(13, 100);
  delay(1000);
}



// void breathe(int pin, int d) {
//   for (int a{}; a < 256; a++) {
//     analogWrite(pin, a);
//     delay(d);
//   }
//   for (int a{ 255 }; a >= 0; a--) {
//     analogWrite(pin, a);
//     delay(d);
//   }
// }
//
// #define westButton 2
// #define eastButton 3
//
// #define eastRed 5
// #define eastYellow 6
// #define eastGreen 7
//
// #define westRed 8
// #define westYellow 9
// #define westGreen 10
//
// #define yellowBlinkTime 500
//
// bool trafficWest = true;
// int flowTime = 10000;
// int changeDelay = 2000;
//
// void setup() {
//   pinMode(westButton, INPUT);
//   pinMode(eastButton, INPUT);
//
//   pinMode(eastRed, OUTPUT);
//   pinMode(eastYellow, OUTPUT);
//   pinMode(eastGreen, OUTPUT);
//
//   pinMode(westRed, OUTPUT);
//   pinMode(westYellow, OUTPUT);
//   pinMode(westGreen, OUTPUT);
//
//   digitalWrite(eastRed, HIGH);
//   digitalWrite(eastYellow, LOW);
//   digitalWrite(eastGreen, LOW);
//
//   digitalWrite(westRed, LOW);
//   digitalWrite(westYellow, LOW);
//   digitalWrite(westGreen, HIGH);
// }
//
// void loop() {
//   if (digitalRead(westButton) == HIGH && trafficWest != true) {
//     trafficWest = true;
//     delay(flowTime);
//     digitalWrite(eastGreen, LOW);
//     digitalWrite(eastYellow, HIGH);
//     delay(changeDelay);
//     digitalWrite(eastYellow, LOW);
//     digitalWrite(eastRed, HIGH);
//     delay(changeDelay);
//     for (int i{}; i < 5; i++) blink(westYellow, yellowBlinkTime);
//     digitalWrite(westYellow, LOW);
//     digitalWrite(westRed, LOW);
//     digitalWrite(westGreen, HIGH);
//   }
//
//   if (digitalRead(eastButton) == HIGH && trafficWest) {
//     trafficWest = false;
//     delay(flowTime);
//     digitalWrite(westGreen, LOW);
//     digitalWrite(westYellow, HIGH);
//     delay(changeDelay);
//     digitalWrite(westYellow, LOW);
//     digitalWrite(westRed, HIGH);
//     delay(changeDelay);
//     for (int i{}; i < 5; i++) blink(eastYellow, yellowBlinkTime);
//     digitalWrite(eastYellow, LOW);
//     digitalWrite(eastRed, LOW);
//     digitalWrite(eastGreen, HIGH);
//   }
// }

// #define NEW 2
// #define OK 4
// #define OLD 6
// int analogVal{};
// float volt{};
// int ledDelay{ 2000 };
//
// void setup() {
//   pinMode(NEW, OUTPUT);
//   pinMode(OK, OUTPUT);
//   pinMode(OLD, OUTPUT);
// }
//
// void loop() {
//   analogVal = analogRead(0);
//   volt = 0.0048 * analogVal;
//   if (volt >= 1.6) blink(NEW, ledDelay);
//   else if (volt < 1.6 && volt > 1.4) blink(OK, ledDelay);
//   else if (volt <= 1.4) blink(OLD, ledDelay);
// }

// void sound(int pin, int duty, int d) {
//   analogWrite(pin, duty);
//   delay(d);
//   digitalWrite(pin, LOW);
//   delay(d);
// }
//
// #define SOUND 6
// int del{ 100 };
// void setup() {
//   pinMode(SOUND, OUTPUT);
// }
//
//
// int pitch{ 25 };
//
// void loop() {
//   sound(SOUND, pitch, 100);
//   pitch = (pitch + 25) % 255;
// }

// int n1{};
// int n2{};
//
// void setup() {
//   Serial.begin(9600);
// }
//
// void loop() {
//   n1 = 0;
//   n2 = 0;
//   Serial.println("Enter two numbers:");
//   while (Serial.available() == 0) {
//   }
//   while (Serial.available() > 0) {
//     n1 = Serial.read() - '0';
//     n2 = Serial.read() - '0';
//   }
//   Serial.print("You entered: ");
//   Serial.print(n1 + " and " + n2);
//   Serial.println();
//   Serial.print(ans);
// }
