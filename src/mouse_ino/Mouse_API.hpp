#pragma once
#include <ArduinoSTL.h>

#include <array>
#include <string>

#include "API.hpp"

using Pins = std::array<uint8_t, 4>;

class Mouse_API : public API {
   private:
    void log(String str) { Serial.println(str); }
    const int del;
    const Pins leftMotorPins;
    const Pins rightMotorPins;

    void clockWise(const Pins& pins) {
        digitalWrite(pins[0], HIGH);
        digitalWrite(pins[1], LOW);
        digitalWrite(pins[2], LOW);
        digitalWrite(pins[3], HIGH);
    }

    void anticlockwise(const Pins& pins) {
        digitalWrite(pins[0], LOW);
        digitalWrite(pins[1], HIGH);
        digitalWrite(pins[2], HIGH);
        digitalWrite(pins[3], LOW);
    }

    void stopPins(const Pins& pins) {
        for (const auto& pin : pins) digitalWrite(pin, LOW);
    }

    void stop() {
        stopPins(leftMotorPins);
        stopPins(rightMotorPins);
    }

   public:
    Mouse_API(const Pins& left, const Pins& right) : API{}, del{500}, leftMotorPins{left}, rightMotorPins{right} {}
    Mouse_API(const Pins& left, const Pins& right, int del)
        : API{}, del{del}, leftMotorPins{left}, rightMotorPins{right} {}

    bool wallFront() override { return false; }
    bool wallRight() override { return false; }
    bool wallLeft() override { return false; }

    void moveForward(int distance = 1) override {
        clockWise(leftMotorPins);
        clockWise(rightMotorPins);
        log("Moved Forward");
        // delay(del);
        // stop();
    }
    void turnRight() override {
        stop();
        delay(del);
        stopPins(leftMotorPins);
        clockWise(rightMotorPins);
        log("Turned Right");
    }
    void turnLeft() override {
        stop();
        delay(del);
        clockWise(leftMotorPins);
        stopPins(rightMotorPins);
        log("Turned Right");
    }
    virtual ~Mouse_API() = default;
};
