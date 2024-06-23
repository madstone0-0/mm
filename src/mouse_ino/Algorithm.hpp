#pragma once

#include <ArduinoSTL.h>

#include <iostream>

#include "API.hpp"

struct Algorithm {
    explicit Algorithm(API* api) : api{api} {}
    virtual void start() = 0;
    virtual ~Algorithm() = default;

   protected:
    API* api;
    void log(const std::string& str) { Serial.println(str.c_str()); }
};
