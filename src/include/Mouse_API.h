#include "API.hpp"

class Mouse_API : public API {
   public:
    bool wallFront() override;
    bool wallRight() override;
    bool wallLeft() override;

    void moveForward(int distance = 1) override;
    void turnRight() override;
    void turnLeft() override;
    virtual ~Mouse_API() = default;
};
