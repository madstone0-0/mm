cmake_minimum_required(VERSION 3.14)

project(mouse LANGUAGES CXX VERSION 1.0)

if(NOT ARDUINO_CMAKE)
    # add_executable(mouse main.cpp ${INCLUDE}/mms_api.hpp ${INCLUDE}/mms_api.cpp ${INCLUDE}/FloodOptimized.hpp ${ARDUINO_INC}/Arduino.h)
    add_executable(mouse main.cpp ${INCLUDE}/mms_api.hpp ${INCLUDE}/mms_api.cpp ${INCLUDE}/FloodOptimized.hpp)
elseif(ARDUINO_CMAKE)
    generate_arduino_firmware(mouse SRCS mouse.cpp HDRS ${INCLUDE}/mms_api.hpp ${INCLUDE}/mms_api.cpp ${INCLUDE}/FloodOptimized.hpp)
elseif(ARDUINO)
    add_executable(mouse main.cpp ${INCLUDE}/mms_api.hpp ${INCLUDE}/mms_api.cpp ${INCLUDE}/FloodOptimized.hpp ${ARDUINO_STL}/ArduinoSTL.h ${ARDUINO_STL}/ArduinoSTL.cpp)
endif()

