#pragma once

#include <iostream>
#include <string>

#include "API.hpp"

using std::string;
class MmsAPI : public API {
   public:
    MmsAPI();
    virtual ~MmsAPI() = default;
    static int mazeWidth();
    static int mazeHeight();

    bool wallFront() override;
    bool wallRight() override;
    bool wallLeft() override;

    void moveForward(int distance = 1) override;
    void turnRight() override;
    void turnLeft() override;

    void setWall(int x, int y, char direction);
    void clearWall(int x, int y, char direction);

    void setColor(int x, int y, char color);
    void clearColor(int x, int y);
    void clearAllColor();

    void setText(int x, int y, const std::string& text);
    void clearText(int x, int y);
    void clearAllText();

    bool wasReset();
    void ackReset();
};
