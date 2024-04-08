#pragma once
#include <format>
#include <ostream>
#include <string>

struct Coords {
    int x;
    int y;

    bool operator==(const Coords& rhs) const { return x == rhs.x && y == rhs.y; }

    // Coords(int x, int y) : x{x}, y{y} {}
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
    // Coords& operator=(Coords other) {
    //     swap(other, *this);
    //     return *this;
    // }

    friend std::ostream& operator<<(std::ostream& s, Coords coords) {
        s << "( " << coords.x << "," << coords.y << " )" << '\n';
        return s;
    }

    operator std::string() const { return std::format("( {}, {} )", x, y); }

   private:
    friend void swap(Coords& first, Coords& second) noexcept {
        using std::swap;
        swap(first.x, second.x);
        swap(first.y, second.y);
    }
};
