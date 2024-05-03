#pragma once

#ifdef ARDUNIO
#include <ArduinoSTL.h>
#else
#include <iostream>
#endif  // ARDUNIO

#include "API.hpp"

struct Algorithm {
    explicit Algorithm(API* api) : api{api} {}
    virtual void start() = 0;
    virtual ~Algorithm() = default;

   protected:
    API* api;
    void log(const std::string& str) { std::cout << str << '\n'; }
};
