#pragma once

#include "Algorithm.hpp"

struct LeftTurn : public Algorithm {
    void start() override {
        log("Start");
        while (true) {
            if (!API::wallLeft()) {
                API::turnLeft();
                log("Left turn");
            }
            while (API::wallFront()) {
                API::turnRight();
                log("Right turn");
            }
            API::moveForward();
        }
    }

    ~LeftTurn() override = default;
};
