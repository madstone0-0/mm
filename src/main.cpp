#include <array>
#include <iostream>
#include <string>

// #include "FloodV2.hpp"
#include "include/Algorithm.hpp"
#include "include/Coords.hpp"
#include "mms_api.hpp"
// #include "include/FloodOptimized.hpp"
#include "FloodMouse.hpp"

using namespace std;

void log(const std::string& text) { std::cout << text << '\n'; }

int main() {
    MmsAPI api{};
    auto mazeDim{Coords{16, 16}};
    auto flood{FloodMouse{&api, mazeDim, vector<Coords>{{7, 7}, {7, 8}, {8, 7}, {8, 8}}}};
    // auto flood{FloodV2{&api, mazeDim, vector<Coords>{{7, 7}, {7, 8}, {8, 7}, {8, 8}}}};

    Algorithm* algo = &flood;

    log("Running");
    algo->start();
}
