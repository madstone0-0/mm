#pragma once
#include <ostream>
#include <string>

#include "Utils.hpp"

struct Coords {
    int x;
    int y;

    bool operator==(const Coords& rhs) const { return x == rhs.x && y == rhs.y; }

    Coords(const int& x, const int& y) : x{x}, y{y} {}
    Coords() : x{}, y{} {}
    //
    // Coords() = default;
    //
    // Coords(Coords&& other) noexcept = default;
    //
    // Coords& operator=(Coords&& other) noexcept = default;
    //
    // Coords(Coords const& other) = default;
    //
    // ~Coords() = default;
    //

    operator std::string() const { return Utils::format("( {}, {} )", x, y); }
};
