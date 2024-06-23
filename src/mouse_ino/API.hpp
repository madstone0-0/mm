#pragma once
class API {
   public:
    virtual bool wallFront() = 0;
    virtual bool wallRight() = 0;
    virtual bool wallLeft() = 0;

    virtual void moveForward(int distance = 1) = 0;
    virtual void turnRight() = 0;
    virtual void turnLeft() = 0;
};
