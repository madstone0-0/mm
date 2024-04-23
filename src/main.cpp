#include <array>
#include <iostream>
#include <string>

#include "Algorithm.hpp"
#include "FloodV2.hpp"
#include "LeftTurn.hpp"

using namespace std;

void log(const std::string& text) { std::cerr << text << '\n'; }

int main() {
    MmsAPI api{};
    auto left{LeftTurn{&api}};
    auto mazeDim{Coords{api.mazeWidth(), api.mazeHeight()}};
    auto flood{FloodV2{&api, mazeDim}};

    Algorithm* algo = &flood;

    log("Running");
    algo->start();
}
