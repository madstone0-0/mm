#pragma once

#include <iostream>

#include "mms_api.hpp"

struct Algorithm {
    virtual void start() = 0;
    virtual ~Algorithm() = default;

   protected:
    void log(const string& str) { std::cerr << str << '\n'; }
};
