#ifdef ARDUNIO
#include <ArduinoSTL.h>
#else
#include <iostream>
#endif  // ARDUNIO

#include <array>
#include <string>

// #include "FloodV2.hpp"
#include "include/Algorithm.hpp"
#include "include/Coords.hpp"
#include "include/FloodOptimized.hpp"

using namespace std;

void log(const std::string& text) { std::cout << text << '\n'; }

int main() {
    MmsAPI api{};
    auto mazeDim{Coords{16, 16}};
    auto flood{FloodOptimized{&api, mazeDim}};

    Algorithm* algo = &flood;

    log("Running");
    algo->start();
}
