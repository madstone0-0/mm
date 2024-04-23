#pragma once

#include <algorithm>
#include <array>
#include <boost/type_index.hpp>
#include <cmath>
#include <exception>
#include <format>
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

#include "Algorithm.hpp"
#include "Coords.hpp"
#include "mms_api.hpp"

enum class Orient { North = 0, East, South, West };

enum class Turn { L = 'L', R = 'R', B = 'B', F = 'F' };

using std::array, std::vector, std::format;
using Maze = vector<vector<int>>;
using Row = vector<int>;
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

    template <typename Base, typename T>
    inline bool instanceof (const T* ptr) {
        auto res = dynamic_cast<const Base*>(ptr) != nullptr;
        return res;
    }

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
    explicit FloodV2(API* api, Coords mazeDim, const vector<Coords>& centers)
        : Algorithm{api},
          mazeDim{mazeDim},
          currPos{},
          cells(mazeDim.y, Row(mazeDim.x, CELL_VAL)),
          flood(mazeDim.y, Row(mazeDim.x, FLOOD_VAL)),
          flood2(mazeDim.y, Row(mazeDim.x, FLOOD_VAL)),
          centers{centers} {}

    explicit FloodV2(API* api, Coords mazeDim)
        : Algorithm{api},
          mazeDim{mazeDim},
          currPos{},
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
        bool mms{ instanceof <MmsAPI>(api)};
        auto wall16 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 'n');
            api->setWall(currPos.x, currPos.y, 'e');
            api->setWall(currPos.x, currPos.y, 'w');
            api->setWall(currPos.x, currPos.y, 's');
        };

        auto wall14 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 'w');
            api->setWall(currPos.x, currPos.y, 'n');
            api->setWall(currPos.x, currPos.y, 's');
        };

        auto wall13 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 'n');
            api->setWall(currPos.x, currPos.y, 'w');
            api->setWall(currPos.x, currPos.y, 'e');
        };

        auto wall12 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 'n');
            api->setWall(currPos.x, currPos.y, 's');
            api->setWall(currPos.x, currPos.y, 'e');
        };

        auto wall11 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 'w');
            api->setWall(currPos.x, currPos.y, 'e');
        };

        auto wall10 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 'n');
            api->setWall(currPos.x, currPos.y, 's');
        };

        auto wall9 = [&wall11](MmsAPI* api) { wall11(api); };

        auto wall8 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 'n');
            api->setWall(currPos.x, currPos.y, 'w');
        };

        auto wall7 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 'n');
            api->setWall(currPos.x, currPos.y, 'e');
        };

        auto wall6 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 's');
            api->setWall(currPos.x, currPos.y, 'e');
        };

        auto wall5 = [this](MmsAPI* api) {
            api->setWall(currPos.x, currPos.y, 's');
            api->setWall(currPos.x, currPos.y, 'w');
        };

        auto wall4 = [this](MmsAPI* api) { api->setWall(currPos.x, currPos.y, 's'); };

        auto wall3 = [this](MmsAPI* api) { api->setWall(currPos.x, currPos.y, 'e'); };

        auto wall2 = [this](MmsAPI* api) { api->setWall(currPos.x, currPos.y, 'n'); };

        auto wall1 = [this](MmsAPI* api) { api->setWall(currPos.x, currPos.y, 'w'); };

        if (L && R && F) {
            if (orient == North) {
                indexCells(currPos) = 13;
                if (mms) {
                    wall13(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == East) {
                indexCells(currPos) = 12;
                if (mms) {
                    wall12(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == South) {
                indexCells(currPos) = 11;
                if (mms) {
                    wall11(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == West) {
                indexCells(currPos) = 14;
                if (mms) {
                    wall14(dynamic_cast<MmsAPI*>(api));
                }
            }
        } else if (L && R && !F) {
            if (orient == North || orient == South) {
                indexCells(currPos) = 9;
                if (mms) {
                    wall9(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == East || orient == West) {
                indexCells(currPos) = 10;
                if (mms) {
                    wall10(dynamic_cast<MmsAPI*>(api));
                }
            }
        } else if (L && !R && F) {
            if (orient == North) {
                indexCells(currPos) = 8;
                if (mms) {
                    wall8(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == East) {
                indexCells(currPos) = 7;
                if (mms) {
                    wall7(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == South) {
                indexCells(currPos) = 6;
                if (mms) {
                    wall6(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == West) {
                indexCells(currPos) = 5;
                if (mms) {
                    wall5(dynamic_cast<MmsAPI*>(api));
                }
            }
        } else if (!L && R && F) {
            if (orient == North) {
                indexCells(currPos) = 7;
                if (mms) {
                    wall7(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == East) {
                indexCells(currPos) = 6;
                if (mms) {
                    wall6(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == South) {
                indexCells(currPos) = 5;
                if (mms) {
                    wall5(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == West) {
                indexCells(currPos) = 8;
                if (mms) {
                    wall8(dynamic_cast<MmsAPI*>(api));
                }
            }
        } else if (F) {
            if (orient == North) {
                indexCells(currPos) = 2;
                if (mms) {
                    wall2(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == East) {
                indexCells(currPos) = 3;
                if (mms) {
                    wall3(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == South) {
                indexCells(currPos) = 4;
                if (mms) {
                    wall4(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == West) {
                indexCells(currPos) = 1;
                if (mms) {
                    wall1(dynamic_cast<MmsAPI*>(api));
                }
            }
        } else if (R) {
            if (orient == North) {
                indexCells(currPos) = 3;
                if (mms) {
                    wall3(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == East) {
                indexCells(currPos) = 4;
                if (mms) {
                    wall4(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == South) {
                indexCells(currPos) = 1;
                if (mms) {
                    wall1(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == West) {
                indexCells(currPos) = 2;
                if (mms) {
                    wall2(dynamic_cast<MmsAPI*>(api));
                }
            }
        } else if (L) {
            if (orient == North) {
                indexCells(currPos) = 1;
                if (mms) {
                    wall1(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == East) {
                indexCells(currPos) = 2;
                if (mms) {
                    wall2(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == South) {
                indexCells(currPos) = 3;
                if (mms) {
                    wall3(dynamic_cast<MmsAPI*>(api));
                }
            } else if (orient == West) {
                indexCells(currPos) = 4;
                if (mms) {
                    wall4(dynamic_cast<MmsAPI*>(api));
                }
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
                // log(format("Surrouding: {} Value: {}", string{s}, indexFlood(s)));
                minVals.at(i) = indexFlood(s);
            }
        }

        auto minVal = minVals.at(0);
        int minCell{};
        int numMov{};

        for (const auto& val : minVals) {
            if (val != 1000) numMov++;
        }

        int currMinI{};
        long currMinVal{};
        for (int i{}; i < 4; i++) {
            const auto& val = minVals.at(i);
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
            const auto& next = surroundings.at(i);
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
                if (static_cast<int>(orient) == -1) orient = West;
                // log(format("Turning: {}", orientToString(orient)));
                break;
            case Turn::R:
                orient = static_cast<Orient>(static_cast<int>(orient) + 1);
                if (static_cast<int>(orient) == 4) orient = North;
                break;
            case Turn::B:
                switch (orient) {
                    case Orient::North:
                        orient = South;
                        // log(format("Turning: {}", orientToString(orient)));
                        break;
                    case Orient::East:
                        orient = West;
                        // log(format("Turning: {}", orientToString(orient)));
                        break;
                    case Orient::South:
                        orient = North;
                        // log(format("Turning: {}", orientToString(orient)));
                        break;
                    case Orient::West:
                        orient = East;
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
            const Coords botRight{0, mazeDim.y - 1};
            const Coords topLeft{mazeDim.x - 1, 0};

            if (indexFlood(currPos) != 0) {
                switch (state) {
                    case 0:
                        appendCenters();
                        // log("In State 0");
                        break;
                    case 1:
                        // appendDest({15, 0});
                        appendDest(topLeft);
                        fast = false;
                        // log("In State 1");
                        break;
                    case 2:
                        appendDest({0, 0});
                        fast = false;
                        // log("In State 2");
                        break;
                    case 3:
                        appendCenters();
                        calcFlood(centers, true);
                        fast = true;
                        // log("In State 3");
                        break;
                    case 4:
                        // appendDest({0, 15});
                        appendDest(botRight);
                        fast = false;
                        // log("In State 4");
                        break;
                    case 5:
                        appendDest({0, 0});
                        fast = false;
                        // log("In State 5");
                        break;
                    case 6:
                        appendCenters();
                        calcFlood(centers, true);
                        fast = true;
                        // log("In State 6");
                        break;
                }
                calcFlood3();
            } else {
                switch (state) {
                    case 6:
                        center();
                        return;
                        break;
                    case 5:
                        appendCenters();
                        calcFlood3();
                        state++;
                        // log("In State 5");
                        break;
                    case 4:
                        changeDestination({0, 0});
                        state++;
                        // log("In State 4");
                        break;
                    case 3:
                        changeDestination(topLeft);
                        state++;
                        // log("In State 3");
                        break;
                    case 2:
                        appendCenters();
                        calcFlood3();
                        state++;
                        // log("In State 2");
                        break;
                    case 1:
                        changeDestination({0, 0});
                        state++;
                        // log("In State 1");
                        break;
                    case 0:
                        center();
                        changeDestination(botRight);
                        calcFlood3();
                        state++;
                        // log("In State 0");
                        break;
                }
                calcFlood(centers, true);
            }

            // if (state == 6) done = true;

            // move to least accessible cell
            Turn direction{};
            // log(format("Fast run: {}", fast));
            if (fast)
                direction = move2();
            else
                direction = move();

            switch (direction) {
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
            show();
            api->moveForward();
            prevPos = currPos;
            updatePos();
            // log(format("Curr pos: {}", string{currPos}));
            //  repeat
        }
    }
};
