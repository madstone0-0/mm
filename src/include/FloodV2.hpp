#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <format>
#include <iostream>
#include <queue>
#include <vector>

#include "Algorithm.hpp"
#include "Coords.hpp"

enum class Orient { North = 0, East, South, West };

enum class Turn { L = 'L', R = 'R', B = 'B', F = 'F' };

using std::array, std::vector, std::format;
using Maze = vector<vector<u_int8_t>>;
using Row = vector<u_int8_t>;
using Orient::East, Orient::North, Orient::South, Orient::West;
namespace rn = std::ranges;

string orientToString(Orient o) {
    if (o == North) return "North";
    if (o == East) return "East";
    if (o == South) return "South";
    if (o == West) return "West";
    return "Error";
}

string turnToString(Turn t) {
    switch (t) {
        case Turn::L:
            return "Left";
            break;
        case Turn::R:
            return "Right";
            break;
        case Turn::B:
            return "Back";
            break;
        case Turn::F:
            return "Forward";
            break;
    }
}

static const int FLOOD_VAL{255};
static const int CELL_VAL{0};
class FloodV2 : public Algorithm {
   private:
    auto& indexFlood(const Coords& c) { return flood.at(c.y).at(c.x); }
    auto& indexFlood2(const Coords& c) { return flood2.at(c.y).at(c.x); }
    [[nodiscard]] bool wall(Coords c) const {
        return (c.x < 0 || c.x > mazeDim.x - 1) || (c.y < 0 || c.y > mazeDim.y - 1);
    }

    auto& indexCells(const Coords& c) { return cells.at(c.y).at(c.x); }

    Coords mazeDim;
    Coords currPos;
    Coords prevPos{};
    Maze cells{};
    Maze flood;
    Maze flood2;
    Orient orient{North};
    std::queue<Coords> moveQueue{};
    vector<Coords> centers;

   public:
    explicit FloodV2(Coords mazeDim, const vector<Coords>& centers)
        : mazeDim{mazeDim},
          currPos{},
          cells(mazeDim.y, Row(mazeDim.x, CELL_VAL)),
          flood(mazeDim.y, Row(mazeDim.x, FLOOD_VAL)),
          flood2(mazeDim.y, Row(mazeDim.x, FLOOD_VAL)),
          centers{centers} {}

    explicit FloodV2(Coords mazeDim)
        : mazeDim{mazeDim},
          currPos{},
          // currPos{0, mazeDim.y - 1},
          cells(mazeDim.y, Row(mazeDim.x, CELL_VAL)),
          flood(mazeDim.y, Row(mazeDim.x, FLOOD_VAL)),
          flood2(mazeDim.y, Row(mazeDim.x, FLOOD_VAL)) {
        Coords midTopLeft{(int)floor(mazeDim.x / 2.0) - 1, (int)floor(mazeDim.y / 2.0) - 1};
        Coords midTopRight{midTopLeft.x + 1, midTopLeft.y};
        Coords midBotLeft{midTopLeft.x, midTopLeft.y + 1};
        Coords midBotRight{midTopLeft.x + 1, midTopLeft.y + 1};
        centers = {midTopLeft, midTopRight, midBotLeft, midBotRight};
    }
    ~FloodV2() override = default;

    vector<Coords> getSurroundings(const Coords& c) {
        vector<Coords> surroundings{};
        vector<Coords> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (auto& d : directions) {
            Coords newCoords{c.x + d.x, c.y + d.y};
            // if (!wall(newCoords)) {
            surroundings.push_back(newCoords);
            // }
        }
        if (directions.at(1).x >= mazeDim.x) directions.at(1).x = -1;
        if (directions.at(0).y >= mazeDim.y) directions.at(0).y = -1;
        return surroundings;
    }

    void show() {
        for (int x{}; x < mazeDim.x; x++) {
            for (int y{}; y < mazeDim.y; y++) {
                int val{indexFlood({x, y})};
                // int val{indexFlood2({x, y})};
                // int val{indexCells({x, y})};
                API::setText(x, y, format("{}", val));
            }
        }
    }

    bool isAccessible(const Coords& s, const Coords& n) {
        if (n.x == mazeDim.x || n.y == mazeDim.y) return false;
        if (s.x == n.x) {
            if (s.y > n.y) {
                if (indexCells(s) == 4 || indexCells(s) == 5 || indexCells(s) == 6 || indexCells(s) == 10 ||
                    indexCells(s) == 11 || indexCells(s) == 12 || indexCells(s) == 14 || indexCells(n) == 2 ||
                    indexCells(n) == 7 || indexCells(n) == 8 || indexCells(n) == 10 || indexCells(n) == 12 ||
                    indexCells(n) == 13 || indexCells(n) == 14)
                    return (false);
                else
                    return (true);
            } else {
                if (indexCells(s) == 2 || indexCells(s) == 7 || indexCells(s) == 8 || indexCells(s) == 10 ||
                    indexCells(s) == 12 || indexCells(s) == 13 || indexCells(s) == 14 || indexCells(n) == 4 ||
                    indexCells(n) == 5 || indexCells(n) == 6 || indexCells(n) == 10 || indexCells(n) == 11 ||
                    indexCells(n) == 12 || indexCells(n) == 14)
                    return (false);
                else
                    return (true);
            }
        } else if (s.y == n.y) {
            if (s.x > n.x) {
                if (indexCells(s) == 1 || indexCells(s) == 5 || indexCells(s) == 8 || indexCells(s) == 9 ||
                    indexCells(s) == 11 || indexCells(s) == 13 || indexCells(s) == 14 || indexCells(n) == 3 ||
                    indexCells(n) == 6 || indexCells(n) == 7 || indexCells(n) == 9 || indexCells(n) == 11 ||
                    indexCells(n) == 12 || indexCells(n) == 13)
                    return (false);
                else
                    return (true);
            } else {
                if (indexCells(s) == 3 || indexCells(s) == 6 || indexCells(s) == 7 || indexCells(s) == 9 ||
                    indexCells(s) == 11 || indexCells(s) == 12 || indexCells(s) == 13 || indexCells(n) == 1 ||
                    indexCells(n) == 5 || indexCells(n) == 8 || indexCells(n) == 9 || indexCells(n) == 11 ||
                    indexCells(n) == 13 || indexCells(n) == 14)
                    return (false);
                else
                    return (true);
            }
            return true;
        }
    }

    void updateWalls(bool L, bool R, bool F) {
        if (L && R && F) {
            if (orient == North) {
                indexCells(currPos) = 13;
            } else if (orient == East) {
                indexCells(currPos) = 12;
            } else if (orient == South) {
                indexCells(currPos) = 11;
            } else if (orient == West) {
                indexCells(currPos) = 14;
            }
        } else if (L && R && !F) {
            if (orient == North || orient == South) {
                indexCells(currPos) = 9;
            } else if (orient == East || orient == West) {
                indexCells(currPos) = 10;
            }
        } else if (L && !R && F) {
            if (orient == North) {
                indexCells(currPos) = 8;
            } else if (orient == East) {
                indexCells(currPos) = 7;
            } else if (orient == South) {
                indexCells(currPos) = 6;
            } else if (orient == West) {
                indexCells(currPos) = 5;
            }
        } else if (!L && R && F) {
            if (orient == North) {
                indexCells(currPos) = 7;
            } else if (orient == East) {
                indexCells(currPos) = 6;
            } else if (orient == South) {
                indexCells(currPos) = 5;
            } else if (orient == West) {
                indexCells(currPos) = 8;
            }
        } else if (F) {
            if (orient == North) {
                indexCells(currPos) = 2;
            } else if (orient == East) {
                indexCells(currPos) = 3;
            } else if (orient == South) {
                indexCells(currPos) = 4;
            } else if (orient == West) {
                indexCells(currPos) = 1;
            }
        } else if (R) {
            if (orient == North) {
                indexCells(currPos) = 3;
            } else if (orient == East) {
                indexCells(currPos) = 4;
            } else if (orient == South) {
                indexCells(currPos) = 1;
            } else if (orient == West) {
                indexCells(currPos) = 2;
            }
        } else if (L) {
            if (orient == North) {
                indexCells(currPos) = 1;
            } else if (orient == East) {
                indexCells(currPos) = 2;
            } else if (orient == South) {
                indexCells(currPos) = 3;
            } else if (orient == West) {
                indexCells(currPos) = 4;
            }
        } else
            indexCells(currPos) = 15;
    }

    void calcFlood(const vector<Coords>& start, bool isTwo = false) {
        for (int i = 0; i < mazeDim.y; i++) {
            for (int j = 0; j < mazeDim.x; j++) {
                if (isTwo)
                    indexFlood2({j, i}) = FLOOD_VAL;

                else
                    indexFlood({j, i}) = FLOOD_VAL;
            }
        }

        std::queue<Coords> q{};
        for (const auto& s : start) {
            q.push(s);
            if (isTwo)
                indexFlood2(s) = 0;

            else
                indexFlood(s) = 0;
        }

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            auto surroundings{getSurroundings(current)};
            for (const auto& next : surroundings) {
                if (wall(next)) continue;
                if (indexCells(next) == 0) continue;
                // if (rn::any_of(start, [&current](Coords c) { return c == current; })) continue;
                if (isTwo) {
                    if (indexFlood2(next) == FLOOD_VAL && isAccessible(current, next)) {
                        indexFlood2(next) = indexFlood2(current) + 1;
                        q.push(next);
                    }
                } else {
                    if (indexFlood(next) == FLOOD_VAL && isAccessible(current, next)) {
                        indexFlood(next) = indexFlood(current) + 1;
                        q.push(next);
                    }
                }
            }
        }
    }

    void calcFlood3() {
        while (!moveQueue.empty()) {
            auto current = moveQueue.front();
            moveQueue.pop();

            auto surroundings{getSurroundings(current)};
            for (const auto& next : surroundings) {
                if (wall(next)) continue;
                if (indexFlood(next) == FLOOD_VAL && isAccessible(current, next)) {
                    indexFlood(next) = indexFlood(current) + 1;
                    moveQueue.push(next);
                }
            }
        }
    }

    Turn move() {
        auto surroundings = getSurroundings(currPos);
        auto construct = indexCells(currPos);
        Orient prev{North};
        vector<long> minVals{1000, 1000, 1000, 1000};

        for (size_t i{}; i < surroundings.size(); i++) {
            const auto& s = surroundings.at(i);
            const auto& x1 = s.x;
            const auto& y1 = s.y;
            if (!wall(s) && isAccessible(currPos, s)) {
                if (x1 == prevPos.x && y1 == prevPos.y) {
                    prev = static_cast<Orient>(i);
                }
                // log(format("Surrouding: {} Value: {}", string{s}, indexFlood(s)));9
                minVals.at(i) = indexFlood(s);
            }
        }

        auto minVal = minVals.at(0);
        int minCell{};
        int numMov{};

        for (const auto& val : minVals) {
            if (val != 1000) numMov++;
        }

        for (int i{}; i < 4; i++) {
            const auto& val = minVals.at(i);
            if (val < minVal) {
                if (numMov == 1) {
                    minVal = val;
                    minCell = i;
                } else {
                    if (i == static_cast<int>(prev))
                        ;
                    else {
                        minVal = val;
                        minCell = i;
                    }
                }
            }
        }

        // log(format("Moving to {}\nOrientation: {}", string{surroundings.at(minCell)}, orientToString(orient)));9
        int orientInt = static_cast<int>(orient);
        // log(format("Orient Int: {}", orientInt));9

        if (minCell == orientInt)
            return Turn::F;
        else if ((minCell == orientInt - 1) || (minCell == orientInt + 3))
            return Turn::L;
        else if ((minCell == orientInt + 1) || (minCell == orientInt - 3))
            return Turn::R;
        else
            return Turn::B;
    }

    Turn move2() {
        auto surroundings = getSurroundings(currPos);
        auto val = indexFlood2(currPos);
        int minCell = 0;
        for (int i{}; i < surroundings.size(); i++) {
            const auto& next = surroundings.at(i);
            if (wall(next)) continue;
            if (isAccessible(currPos, next)) {
                if (indexFlood2(next) == val - 1) minCell = i;
            }
        }

        if (minCell == static_cast<int>(orient))
            return Turn::F;
        else if ((minCell == static_cast<int>(orient) - 1) || (minCell == static_cast<int>(orient) + 3))
            return Turn::L;
        else if ((minCell == static_cast<int>(orient) + 1) || (minCell == static_cast<int>(orient) - 3))
            return Turn::R;
        else
            return Turn::B;
    }

    void appendCenters() {
        for (int i = 0; i < mazeDim.y; i++) {
            for (int j = 0; j < mazeDim.x; j++) {
                indexFlood({j, i}) = FLOOD_VAL;
            }
        }

        for (const auto& c : centers) {
            indexFlood(c) = 0;
            moveQueue.push(c);
        }
    }

    void appendDest(const Coords& dest) {
        for (int i = 0; i < mazeDim.y; i++) {
            for (int j = 0; j < mazeDim.x; j++) {
                indexFlood({j, i}) = FLOOD_VAL;
            }
        }

        indexFlood(dest) = 0;
        moveQueue.push(dest);
    }

    void printMouse() {
        for (int i{}; i < flood.size(); ++i) {
            for (int j{}; j < flood.size(); ++j) {
                std::cout << std::format("{:2}", indexFlood({j, i})) << ' ';
            }
            std::cout << '\n';
        }
    }

    void center() {
        auto L = API::wallLeft();
        auto R = API::wallRight();
        auto F = API::wallFront();

        if (L) {
            updateWalls(L, R, F);

            for (int i{}; i < 4; i++) {
                API::moveForward();

                prevPos = currPos;
                updatePos();
                L = API::wallLeft();
                R = API::wallRight();
                F = API::wallFront();
                updateWalls(L, R, F);

                API::turnRight();
                orientation(Turn::R);
            }
        } else {
            updateWalls(L, R, F);

            for (int i{}; i < 4; i++) {
                API::moveForward();

                prevPos = currPos;
                updatePos();
                L = API::wallLeft();
                R = API::wallRight();
                F = API::wallFront();
                updateWalls(L, R, F);

                API::turnLeft();
                orientation(Turn::L);
            }
        }
    }

    void orientation(Turn t) {
        switch (t) {
            case Turn::L:
                orient = static_cast<Orient>(static_cast<int>(orient) - 1);
                if (static_cast<int>(orient) == -1) orient = West;
                // log(format("Turning: {}", orientToString(orient)));9
                break;
            case Turn::R:
                orient = static_cast<Orient>(static_cast<int>(orient) + 1);
                if (static_cast<int>(orient) == 4) orient = North;
                break;
            case Turn::B:
                switch (orient) {
                    case Orient::North:
                        orient = South;
                        // log(format("Turning: {}", orientToString(orient)));9
                        break;
                    case Orient::East:
                        orient = West;
                        // log(format("Turning: {}", orientToString(orient)));9
                        break;
                    case Orient::South:
                        orient = North;
                        // log(format("Turning: {}", orientToString(orient)));9
                        break;
                    case Orient::West:
                        orient = East;
                        // log(format("Turning: {}", orientToString(orient)));9
                        break;
                }
                break;
            case Turn::F:
                break;
        }
    }

    void updatePos() {
        switch (orient) {
            case Orient::North:
                currPos.y++;
                break;
            case Orient::East:
                currPos.x++;
                break;
            case Orient::South:
                currPos.y--;
                break;
            case Orient::West:
                currPos.x--;
                break;
        }
    }

    void changeDestination(const Coords& dest) {
        for (int i = 0; i < mazeDim.y; i++) {
            for (int j = 0; j < mazeDim.x; j++) {
                indexFlood({j, i}) = FLOOD_VAL;
            }
        }

        std::queue<Coords> q;
        indexFlood(dest) = 0;

        q.push(dest);

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            auto surroundings{getSurroundings(current)};
            for (const auto& next : surroundings) {
                if (wall(next)) continue;
                if (indexFlood(next) == FLOOD_VAL) {
                    indexFlood(next) = indexFlood(current) + 1;
                    q.push(next);
                }
            }
        }
    }

    void start() override {
        int state = 0;
        bool fast{};
        bool done{false};
        int ticksInSix{};

        calcFlood(centers);
        while (!done) {
            API::setColor(currPos.x, currPos.y, 'R');
            auto L = API::wallLeft();
            auto R = API::wallRight();
            auto F = API::wallFront();
            // check for walls
            // update walls
            updateWalls(L, R, F);

            if (indexFlood(currPos) != 0) {
                switch (state) {
                    case 0:
                        appendCenters();
                        break;
                    case 1:
                        appendDest({15, 0});
                        fast = false;
                        break;
                    case 2:
                        appendDest({0, 0});
                        fast = false;
                        break;
                    case 3:
                        appendCenters();
                        calcFlood(centers, true);
                        fast = true;
                        break;
                    case 4:
                        appendDest({0, 15});
                        fast = false;
                        break;
                    case 5:
                        appendDest({0, 0});
                        fast = false;
                        break;
                    case 6:
                        appendCenters();
                        calcFlood(centers, true);
                        fast = true;
                        break;
                }
                calcFlood3();
            } else {
                switch (state) {
                    case 5:
                        appendCenters();
                        calcFlood3();
                        state++;
                        break;
                    case 4:
                        changeDestination({0, 0});
                        state++;
                        break;
                    case 3:
                        changeDestination({15, 0});
                        state++;
                        break;
                    case 2:
                        appendCenters();
                        calcFlood3();
                        state++;
                        break;
                    case 1:
                        changeDestination({0, 0});
                        state++;
                        break;
                    case 0:
                        center();
                        changeDestination({0, 15});
                        state++;
                        break;
                }
                calcFlood(centers, true);
            }

            // if (state == 6) done = true;

            // move to least accessible cell
            Turn direction{};
            // log(format("Fast run: {}", fast));9
            if (fast)
                direction = move2();
            else
                direction = move();

            switch (direction) {
                case Turn::L:
                    API::turnLeft();
                    // log(format("Need to turn: {}", turnToString(Turn::L)));9
                    orientation(Turn::L);
                    break;
                case Turn::R:
                    API::turnRight();
                    // log(format("Need to turn: {}", turnToString(Turn::R)));9
                    orientation(Turn::R);
                    break;
                case Turn::B:
                    API::turnLeft();
                    orientation(Turn::L);
                    // log(format("Need to turn: {}", turnToString(Turn::L)));9
                    API::turnLeft();
                    // log(format("Need to turn: {}", turnToString(Turn::L)));9
                    orientation(Turn::L);
                    break;
                case Turn::F:
                    break;
            }
            show();
            API::moveForward();
            prevPos = currPos;
            updatePos();
            // log(format("Curr pos: {}", string{currPos}));9
            //  repeat
            if (state == 6) {
                ticksInSix++;
            }
            if (ticksInSix >= 100) done = true;
        }
    }
};
