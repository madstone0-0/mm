#include "API.hpp"
class Mouse_API : public API {
   public:
    bool wallFront() override { return false; }
    bool wallRight() override { return false; }
    bool wallLeft() override { return false; }

    void moveForward(int distance = 1) override {}
    void turnRight() override {}
    void turnLeft() override {}
    virtual ~Mouse_API() = default;
};
