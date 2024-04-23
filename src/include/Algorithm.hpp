#pragma once

#include <iostream>

#include "API.hpp"
#include "mms_api.hpp"

struct Algorithm {
    explicit Algorithm(API* api) : api{api} {}
    virtual void start() = 0;
    virtual ~Algorithm() = default;

   protected:
    API* api;
    void log(const string& str) { std::cerr << str << '\n'; }
};
