#pragma once

#include "Algorithm.hpp"

struct LeftTurn : public Algorithm {
    LeftTurn(API* api) : Algorithm(api) {}

    void start() override {
        log("Start");
        while (true) {
            if (api->wallLeft()) {
                api->turnLeft();
                log("Left turn");
            }
            while (api->wallFront()) {
                api->turnRight();
                log("Right turn");
            }
            api->moveForward();
        }
    }

    ~LeftTurn() override = default;
};
