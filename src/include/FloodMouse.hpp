#pragma once

#include <array>
#include <cstdint>
#include <iterator>
#include <queue>
#include <vector>

#include "Algorithm.hpp"
#include "Coords.hpp"
#include "Utils.hpp"
#include "mms_api.hpp"

#define IMPL LIFO

enum class Orient { North = 0, East, South, West };

enum class Turn { L = 'L', R = 'R', B = 'B', F = 'F' };

using std::array, std::vector, Utils::format;
// using Maze = vector<vector<int>>;
// using Maze = uint8_t[16][16];
// using Maze = uint8_t[16][16];
using Maze = array<array<uint8_t, 16>, 16>;

static const uint8_t FLOOD_VAL{200};
static const uint8_t CELL_VAL{0};

class FloodMouse : public Algorithm {
   private:
    int indexFlood(Coords c) { return flood[c.y][c.x]; }
    int indexFlood2(Coords c) { return flood2[c.y][c.x]; }
    void setFlood(Coords c, int val) { flood[c.y][c.x] = val; }
    void setFlood2(Coords c, int val) { flood2[c.y][c.x] = val; }
    [[nodiscard]] bool wall(Coords c) const {
        return (c.x < 0 || c.x > mazeDim.x - 1) || (c.y < 0 || c.y > mazeDim.y - 1);
    }

    auto& indexCells(Coords c) { return cells[c.y][c.x]; }

    Coords mazeDim{};
    Coords currPos{};
    Coords prevPos{};
    Maze cells{};
    Maze flood{};
    Maze flood2{};
    Orient orient{Orient::North};
    // cppQueue moveQueue{sizeof(Coords), 256};
    std::queue<Coords> moveQueue{};
    vector<Coords> centers;

   public:
    explicit FloodMouse(API* api, Coords mazeDim, const vector<Coords>& centers)
        : Algorithm{api}, mazeDim{mazeDim}, centers{centers} {
        for (int y = 0; y < mazeDim.y; y++)
            for (int x = 0; x < mazeDim.x; x++) {
                cells[y][x] = CELL_VAL;
                flood[y][x] = FLOOD_VAL;
                flood2[y][x] = FLOOD_VAL;
            }
    }
    ~FloodMouse() override = default;

    [[nodiscard]] vector<Coords> getSurroundings(const Coords& c) const {
        vector<Coords> surroundings{};
        vector<Coords> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        for (auto& d : directions) {
            Coords newCoords{c.x + d.x, c.y + d.y};
            surroundings.push_back(newCoords);
        }

        if (directions.at(1).x >= mazeDim.x) directions.at(1).x = -1;
        if (directions.at(0).y >= mazeDim.y) directions.at(0).y = -1;
        return surroundings;
    }

    /**
     * @brief Return whether a proposed coordinate is accessible by checking the type of wall it is,
     * @param Current Coordinates
     * @param Proposed next Coordinates
     * Where when the mouse is Facing north :
     * 1 - W
     * 2 - N
     * 3 - E
     * 4 - S
     * 5 - W S
     * 6 - E S
     * 7 - E N
     * 8 - W N
     * 9 - W E
     * 10 - N S
     * 11 - W E
     * 12 - E N S
     * 13 - W E N
     * 14 - W N S
     * 15 - No walls
     * 16 - N S W E
     */
    bool isAccessible(const Coords& c, const Coords& n) {
        if (n.x == mazeDim.x || n.y == mazeDim.y) return false;
        if (c.x == n.x) {
            if (c.y > n.y) {
                return indexCells(c) != 4 && indexCells(c) != 5 && indexCells(c) != 6 && indexCells(c) != 10 &&
                       indexCells(c) != 11 && indexCells(c) != 12 && indexCells(c) != 14 && indexCells(n) != 2 &&
                       indexCells(n) != 7 && indexCells(n) != 8 && indexCells(n) != 10 && indexCells(n) != 12 &&
                       indexCells(n) != 13 && indexCells(n) != 14;
            } else {
                return indexCells(c) != 2 && indexCells(c) != 7 && indexCells(c) != 8 && indexCells(c) != 10 &&
                       indexCells(c) != 12 && indexCells(c) != 13 && indexCells(c) != 14 && indexCells(n) != 4 &&
                       indexCells(n) != 5 && indexCells(n) != 6 && indexCells(n) != 10 && indexCells(n) != 11 &&
                       indexCells(n) != 12 && indexCells(n) != 14;
            }
        } else if (c.y == n.y) {
            if (c.x > n.x) {
                return indexCells(c) != 1 && indexCells(c) != 5 && indexCells(c) != 8 && indexCells(c) != 9 &&
                       indexCells(c) != 11 && indexCells(c) != 13 && indexCells(c) != 14 && indexCells(n) != 3 &&
                       indexCells(n) != 6 && indexCells(n) != 7 && indexCells(n) != 9 && indexCells(n) != 11 &&
                       indexCells(n) != 12 && indexCells(n) != 13;
            } else {
                return indexCells(c) != 3 && indexCells(c) != 6 && indexCells(c) != 7 && indexCells(c) != 9 &&
                       indexCells(c) != 11 && indexCells(c) != 12 && indexCells(c) != 13 && indexCells(n) != 1 &&
                       indexCells(n) != 5 && indexCells(n) != 8 && indexCells(n) != 9 && indexCells(n) != 11 &&
                       indexCells(n) != 13 && indexCells(n) != 14;
            }
            return true;
        }
    }

    void updateWalls(bool L, bool R, bool F) {
        if (L && R && F) {
            if (orient == Orient::North) {
                indexCells(currPos) = 13;
            } else if (orient == Orient::East) {
                indexCells(currPos) = 12;
            } else if (orient == Orient::South) {
                indexCells(currPos) = 11;
            } else if (orient == Orient::West) {
                indexCells(currPos) = 14;
            }
        } else if (L && R && !F) {
            if (orient == Orient::North || orient == Orient::South) {
                indexCells(currPos) = 9;
            } else if (orient == Orient::East || orient == Orient::West) {
                indexCells(currPos) = 10;
            }
        } else if (L && !R && F) {
            if (orient == Orient::North) {
                indexCells(currPos) = 8;
            } else if (orient == Orient::East) {
                indexCells(currPos) = 7;
            } else if (orient == Orient::South) {
                indexCells(currPos) = 6;
            } else if (orient == Orient::West) {
                indexCells(currPos) = 5;
            }
        } else if (!L && R && F) {
            if (orient == Orient::North) {
                indexCells(currPos) = 7;
            } else if (orient == Orient::East) {
                indexCells(currPos) = 6;
            } else if (orient == Orient::South) {
                indexCells(currPos) = 5;
            } else if (orient == Orient::West) {
                indexCells(currPos) = 8;
            }
        } else if (F) {
            if (orient == Orient::North) {
                indexCells(currPos) = 2;
            } else if (orient == Orient::East) {
                indexCells(currPos) = 3;
            } else if (orient == Orient::South) {
                indexCells(currPos) = 4;
            } else if (orient == Orient::West) {
                indexCells(currPos) = 1;
            }
        } else if (R) {
            if (orient == Orient::North) {
                indexCells(currPos) = 3;
            } else if (orient == Orient::East) {
                indexCells(currPos) = 4;
            } else if (orient == Orient::South) {
                indexCells(currPos) = 1;
            } else if (orient == Orient::West) {
                indexCells(currPos) = 2;
            }
        } else if (L) {
            if (orient == Orient::North) {
                indexCells(currPos) = 1;
            } else if (orient == Orient::East) {
                indexCells(currPos) = 2;
            } else if (orient == Orient::South) {
                indexCells(currPos) = 3;
            } else if (orient == Orient::West) {
                indexCells(currPos) = 4;
            }
        } else
            indexCells(currPos) = 15;
    }

    /*
      Calculate Flood algorithm for either initial run or 2nd run
      */
    void calcFlood(const vector<Coords>& start, bool isTwo = false) {
        for (int i = 0; i < mazeDim.y; i++) {
            for (int j = 0; j < mazeDim.x; j++) {
                if (isTwo)
                    setFlood2({j, i}, FLOOD_VAL);

                else
                    setFlood({j, i}, FLOOD_VAL);
            }
        }

        std::queue<Coords> q{};
        for (const Coords& s : start) {
            q.push(s);
            if (isTwo)
                setFlood2(s, 0);

            else
                setFlood(s, 0);
        }

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            auto surroundings{getSurroundings(current)};
            for (const Coords& next : surroundings) {
                if (wall(next)) continue;
                if (indexCells(next) == 0) continue;
                if (isTwo) {
                    if (indexFlood2(next) == FLOOD_VAL && isAccessible(current, next)) {
                        setFlood2(next, indexFlood2(current) + 1);
                        q.push(next);
                    }
                } else {
                    if (indexFlood(next) == FLOOD_VAL && isAccessible(current, next)) {
                        setFlood(next, indexFlood(current) + 1);
                        q.push(next);
                    }
                }
            }
        }
    }

    void calcFlood3() {
        // while (!moveQueue.isEmpty()) {
        while (!moveQueue.empty()) {
            // Coords current;
            Coords current = moveQueue.front();
            // moveQueue.pop(&current);
            moveQueue.pop();

            auto surroundings{getSurroundings(current)};
            for (auto next : surroundings) {
                if (wall(next)) continue;
                if (indexFlood(next) == FLOOD_VAL && isAccessible(current, next)) {
                    setFlood(next, indexFlood(current) + 1);
                    // moveQueue.push(&next);
                    moveQueue.push(next);
                }
            }
        }
    }

    Turn move() {
        auto surroundings = getSurroundings(currPos);
        Orient prev{Orient::North};
        vector<long> minVals{1000, 1000, 1000, 1000};

        for (size_t i{}; i < surroundings.size(); i++) {
            auto s = surroundings.at(i);
            auto x1 = s.x;
            auto y1 = s.y;
            if (!wall(s) && isAccessible(currPos, s)) {
                if (x1 == prevPos.x && y1 == prevPos.y) {
                    prev = static_cast<Orient>(i);
                }
                // log(format("Surrouding: {} Value: {}", string{s}, indexFlood(s)));
                minVals.at(i) = indexFlood(s);
            }
        }

        auto minVal = minVals.at(0);
        int minCell{};
        int numMov{};

        for (auto val : minVals) {
            if (val != 1000) numMov++;
        }

        int currMinI{};
        long currMinVal{};
        for (int i{}; i < 4; i++) {
            auto val = minVals.at(i);
            if (val <= minVal) {
                currMinI = i;
                currMinVal = val;
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

                } else if (val == minVal) {
                    if (numMov == 1) {
                        minVal = val;
                        minCell = i;
                    } else {
                        if (currMinI == static_cast<int>(orient)) {
                            minVal = currMinVal;
                            minCell = currMinI;
                        }
                    }
                }
            }
        }

        // log(format("Moving to {}\nOrientation: {}", string{surroundings.at(minCell)}, orientToString(orient)));
        int orientInt = static_cast<int>(orient);
        // log(format("Orient Int: {}", orientInt));

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
            auto next = surroundings.at(i);
            if (wall(next)) continue;
            if (isAccessible(currPos, next)) {
                if (indexFlood2(next) == val - 1) minCell = i;
            }
        }

        int orientInt = static_cast<int>(orient);

        if (minCell == orientInt)
            return Turn::F;
        else if ((minCell == orientInt - 1) || (minCell == orientInt + 3))
            return Turn::L;
        else if ((minCell == orientInt + 1) || (minCell == orientInt - 3))
            return Turn::R;
        else
            return Turn::B;
    }

    void appendCenters() {
        for (int i = 0; i < mazeDim.y; i++) {
            for (int j = 0; j < mazeDim.x; j++) {
                setFlood({j, i}, FLOOD_VAL);
            }
        }

        for (const auto& c : centers) {
            setFlood(c, 0);
            // moveQueue.push(&c);
            moveQueue.push(c);
        }

        // for (Coords c : centers) {
        //     setFlood(c, 0);
        //     moveQueue.push(c);
        // }
    }

    void appendDest(const Coords& dest) {
        for (int i = 0; i < mazeDim.y; i++) {
            for (int j = 0; j < mazeDim.x; j++) {
                setFlood({j, i}, FLOOD_VAL);
            }
        }

        setFlood(dest, 0);
        // moveQueue.push(&dest);
        moveQueue.push(dest);
    }

    void center() {
        auto L = api->wallLeft();
        auto R = api->wallRight();
        auto F = api->wallFront();

        if (L) {
            updateWalls(L, R, F);

            for (int i{}; i < 4; i++) {
                api->moveForward();

                prevPos = currPos;
                updatePos();
                L = api->wallLeft();
                R = api->wallRight();
                F = api->wallFront();
                updateWalls(L, R, F);

                api->turnRight();
                orientation(Turn::R);
            }
        } else {
            updateWalls(L, R, F);

            for (int i{}; i < 4; i++) {
                api->moveForward();

                prevPos = currPos;
                updatePos();
                L = api->wallLeft();
                R = api->wallRight();
                F = api->wallFront();
                updateWalls(L, R, F);

                api->turnLeft();
                orientation(Turn::L);
            }
        }
    }

    void orientation(Turn t) {
        switch (t) {
            case Turn::L:
                orient = static_cast<Orient>(static_cast<int>(orient) - 1);
                if (static_cast<int>(orient) == -1) orient = Orient::West;
                // log(format("Turning: {}", orientToString(orient)));
                break;
            case Turn::R:
                orient = static_cast<Orient>(static_cast<int>(orient) + 1);
                if (static_cast<int>(orient) == 4) orient = Orient::North;
                break;
            case Turn::B:
                switch (orient) {
                    case Orient::North:
                        orient = Orient::South;
                        // log(format("Turning: {}", orientToString(orient)));
                        break;
                    case Orient::East:
                        orient = Orient::West;
                        // log(format("Turning: {}", orientToString(orient)));
                        break;
                    case Orient::South:
                        orient = Orient::North;
                        // log(format("Turning: {}", orientToString(orient)));
                        break;
                    case Orient::West:
                        orient = Orient::East;
                        // log(format("Turning: {}", orientToString(orient)));
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
                setFlood({j, i}, FLOOD_VAL);
            }
        }
        // log(format("Dest to ({}, {})", dest.x, dest.y).c_str());

        // cppQueue q{sizeof(Coords), 50, IMPL};
        std::queue<Coords> q{};
        setFlood(dest, 0);

        // q.push(&dest);
        q.push(dest);

        // while (!q.isEmpty()) {
        while (!q.empty()) {
            // Coords current;
            // q.pop(&current);

            Coords current = q.front();
            q.pop();

            auto surroundings{getSurroundings(current)};
            for (auto next : surroundings) {
                if (wall(next)) continue;
                if (indexFlood(next) == FLOOD_VAL) {
                    setFlood(next, indexFlood(current) + 1);
                    // q.push(&next);
                    q.push(next);
                }
            }
        }
    }

    int isBuggyCenter() {
        auto topLeft{centers.at(2)};
        auto topRight{centers.at(3)};
        const auto isTopLeftExit = [this, &topLeft]() { return indexCells(topLeft) == 2; };
        const auto isTopRightExit = [this, &topRight]() { return indexCells(topRight) == 2; };
        if (isTopLeftExit())
            return 1;
        else if (isTopRightExit())
            return 2;
        else
            return 0;
    }

    template <typename Itr, typename Func>
    bool any_of(Itr beg, Itr end, Func f) {
        while (beg != end) {
            if (f(*beg)) return true;
            ++beg;
        }
        return false;
    }

    bool isStuck() {
        auto surroundings{getSurroundings(currPos)};
        return any_of(surroundings.cbegin(), surroundings.cend(), [this](auto s) {
            if (!wall(s) && isAccessible(currPos, s)) return indexFlood(s) == FLOOD_VAL;
            return false;
        });
    }

    void checkBugged(bool& bugged, int buggy, const Coords& dest) {
        bugged = true;
        changeDestination(dest);
        if (buggy == 1 && orient == Orient::East) {
            api->turnLeft();
            orientation(Turn::L);
            api->turnLeft();
            orientation(Turn::L);
        } else if (buggy == 2 && orient == Orient::West) {
            api->turnRight();
            orientation(Turn::R);
            api->turnRight();
            orientation(Turn::R);
        }
    }

    template <typename Base, typename T>
    inline bool instanceof (const T* ptr) {
        auto res = dynamic_cast<const Base*>(ptr) != nullptr;
        return res;
    }

    void show() {
        for (int x{}; x < mazeDim.x; x++) {
            for (int y{}; y < mazeDim.y; y++) {
                int val{indexFlood({x, y})};
                // int val{indexFlood2({x, y})};
                // int val{indexCells({x, y})};
                // api->setText(x, y, format("{}", val));
                if (instanceof <MmsAPI>(api)) {
                    dynamic_cast<MmsAPI*>(api)->setText(x, y, format("{}", val));
                }
            }
        }
    }

    void start() override {
        int state{};
        bool fast{};
        bool goingToStart{};
        bool bugged{};
        uint8_t buggy{};

        auto backToStart = [this, &goingToStart]() {
            log("Stuck!");
            changeDestination({0, 0});
            calcFlood(centers);
            goingToStart = true;
        };

        // Serial.println("In Start");
        calcFlood(centers);
        while (true) {
            if (instanceof <MmsAPI>(api)) {
                dynamic_cast<MmsAPI*>(api)->setColor(currPos.x, currPos.y, 'B');
            }
            auto L = api->wallLeft();
            auto R = api->wallRight();
            auto F = api->wallFront();
            // check for walls
            // update walls
            updateWalls(L, R, F);
            const Coords tpL{0, mazeDim.y - 1};
            const Coords btmR{mazeDim.x - 1, 0};

            if (indexFlood(currPos) != 0) {
                switch (state) {
                    case 0:
                        appendCenters();
                        log("In State 0");
                        break;
                    case 1:
                        // appendDest({15, 0});
                        if (!goingToStart) {
                            if (!isStuck()) {
                                appendDest(btmR);
                            } else
                                backToStart();
                        }

                        fast = false;
                        log("In State 1");
                        break;
                    case 2:
                        appendDest({0, 0});
                        fast = false;
                        log("In State 2");
                        break;
                    case 3:
                        appendCenters();
                        calcFlood(centers, true);
                        fast = true;
                        log("In State 3");
                        break;
                    case 4:
                        // appendDest({0, 15});
                        if (!goingToStart) {
                            if (!isStuck())
                                appendDest(tpL);
                            else
                                backToStart();
                        }
                        fast = false;
                        log("In State 4");
                        break;
                    case 5:
                        appendDest({0, 0});
                        fast = false;
                        log("In State 5");
                        break;
                    case 6:
                        appendCenters();
                        calcFlood(centers, true);
                        fast = true;
                        log("In State 6");
                        break;
                    case 7:
                        appendDest({0, 0});
                        fast = false;
                        log("In state 7");
                        break;
                }
                calcFlood3();
            } else {
                switch (state) {
                    case 7:
                        return;
                        break;
                    case 6:
                        center();
                        if (buggy != 0)
                            checkBugged(bugged, buggy, {0, 0});
                        else
                            changeDestination({0, 0});
                        calcFlood3();
                        state++;
                        break;
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
                        if (buggy != 0)
                            checkBugged(bugged, buggy, btmR);
                        else
                            changeDestination(btmR);
                        state++;
                        break;
                    case 2:
                        appendCenters();
                        calcFlood3();
                        state++;
                        break;
                    case 1:
                        goingToStart = false;
                        changeDestination({0, 0});
                        state++;
                        break;
                    case 0:
                        log("On 0 in state 0");
                        center();
                        changeDestination(tpL);
                        calcFlood3();
                        buggy = isBuggyCenter();
                        state++;
                        break;
                }
                calcFlood(centers, true);
            }

            // // if (state == 6) done = true;

            // move to least accessible cell
            Turn direction{};
            // log(format("Fast run: {}", fast));
            if (fast)
                direction = move2();
            else
                direction = move();
            if (!bugged) switch (direction) {
                    case Turn::L:
                        api->turnLeft();
                        // log(format("Need to turn: {}", turnToString(Turn::L)));
                        orientation(Turn::L);
                        break;
                    case Turn::R:
                        api->turnRight();
                        // log(format("Need to turn: {}", turnToString(Turn::R)));
                        orientation(Turn::R);
                        break;
                    case Turn::B:
                        api->turnLeft();
                        orientation(Turn::L);
                        // log(format("Need to turn: {}", turnToString(Turn::L)));
                        api->turnLeft();
                        // log(format("Need to turn: {}", turnToString(Turn::L)));
                        orientation(Turn::L);
                        break;
                    case Turn::F:
                        break;
                }
            else
                bugged = false;
            show();
            api->moveForward();
            prevPos = currPos;
            updatePos();
            // log(format("Curr pos: {}", string{currPos}));
            //  repeat
        }
    }
};
