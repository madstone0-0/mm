#include "Arduino.h"
#include "WString.h"
#pragma once
#include <ArduinoSTL.h>

#include <array>
#include <string>

#include "API.hpp"

using Pins = std::array<uint8_t, 4>;

template <typename T>
using LRPair = std::array<T, 2>;

class Mouse_API : public API {
   private:
    void logS(String str) { Serial.println(str); }

    void log(const std::string& str) { Serial.println(str.c_str()); }

    const int del{};
    int count{};
    const Pins leftMotorPins{};
    const Pins rightMotorPins{};
    const std::array<uint8_t, 2> encoderPinsLR{};
    LRPair<bool> encoderValsLR{};
    LRPair<bool> encoderPreValsLR{};
    LRPair<int> encoderPosLR{};
    int vel{};
    unsigned long lastTick{};

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

    void handleLeftRising() {
        // TODO Need to check encoder behaviour on wheel turn
        if (encoderValsLR.at(0) == HIGH) encoderPosLR.at(0)++;

        // TODO Find the number of seconds since the last rising edge then store its reciprocal to velocity
        auto timeSinceTick = micros() - lastTick;
        lastTick = micros();
        vel = (int)((1. / timeSinceTick) * 100);
    }

    void handleRightRising() {
        // TODO Need to check encoder behaviour on wheel turn
        if (encoderValsLR.at(1) == HIGH) encoderPosLR.at(1)++;

        // TODO Find the number of seconds since the last rising edge then store its reciprocal to velocity
        auto timeSinceTick = micros() - lastTick;
        lastTick = micros();
        vel = (int)((1. / timeSinceTick) * 100);
    }

    static Mouse_API* instance;

   public:
    Mouse_API(const Pins& left, const Pins& right, const decltype(encoderPinsLR) encoderPinsLR)
        : API{}, del{100}, leftMotorPins{left}, rightMotorPins{right}, encoderPinsLR{encoderPinsLR} {
        instance = this;
    }
    Mouse_API(const Pins& left, const Pins& right, const decltype(encoderPinsLR) encoderPinsLR, int del)
        : API{}, del{del}, leftMotorPins{left}, rightMotorPins{right}, encoderPinsLR{encoderPinsLR} {
        instance = this;
    }

    void setupInterrupts() {
        log("Setting up interrupts");
        attachInterrupt(digitalPinToInterrupt(encoderPinsLR.at(0)), leftEncoderRisingEdge, RISING);
        attachInterrupt(digitalPinToInterrupt(encoderPinsLR.at(1)), rightEncoderRisingEdge, RISING);
    }

    bool wallFront() override { return false; }
    bool wallRight() override { return false; }
    bool wallLeft() override { return false; }

    void loop() override {
        encoderValsLR.at(0) = digitalRead(encoderPinsLR.at(0));
        encoderValsLR.at(1) = digitalRead(encoderPinsLR.at(1));

        if (count % 100 == 0) {
            logS(String("L: ") + String(encoderPosLR.at(0)));
            logS(String("R: ") + String(encoderPosLR.at(1)));
            logS(String("Velocity: ") + String{vel});
        }
        count++;
    }

    static void leftEncoderRisingEdge() {
        if (instance) {
            instance->handleLeftRising();
        }
    }

    static void rightEncoderRisingEdge() {
        if (instance) {
            instance->handleRightRising();
        }
    }

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

Mouse_API* Mouse_API::instance = nullptr;
