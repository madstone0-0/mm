#include <array>
#include <iostream>
#include <string>

#include "Algorithm.hpp"
#include "FloodV2.hpp"
#include "LeftTurn.hpp"

using namespace std;

void log(const std::string& text) { std::cerr << text << '\n'; }

int main() {
    auto left{LeftTurn{}};
    auto mazeDim{Coords{16, 16}};
    auto flood{FloodV2{mazeDim}};

    Algorithm* algo = &flood;

    log("Running");
    algo->start();
}
