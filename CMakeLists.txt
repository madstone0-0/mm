cmake_minimum_required(VERSION 3.14)

project(mouse LANGUAGES CXX C VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
option(ARDUINO "Arduino" OFF)
message("Arduino: ${ARDUINO}")
if(NOT ARDUINO) 
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_C_FLAGS "-lncursesw")
    set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -fsanitize=address")
# set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -fno-ident -ftree-vectorize -ffast-math -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,-s -flto")
else()
    set(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake) # Arduino Toolchain
    include(cmake/Platform/Arduino.cmake)
    print_board_list()
    print_programmer_list()
    set(ARDUINO_DEFAULT_BOARD uno) # Default Board ID
    set(ARDUINO_DEFAULT_PORT /dev/ttyACM0) # Default Port
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add_compile_options(
# "-Wall" "-Wextra" "-std=c++20" "-fmodules-ts" "-fconcepts"
# "$<$<CONFIG:DEBUG>: -g3;-ggdb;-Os>"
# "$<$<CONFIG:RELEASE>: -O3;-Os;-fno-ident>"
#     )
# include_directories("/usr/include/python3.11")
set(INCLUDE ${CMAKE_SOURCE_DIR}/src/include)
include_directories(${INCLUDE})
set(UTILS ${INCLUDE}/utils.hpp)
set(THIRD_PARTY ${INCLUDE}/third_party)
# file(GLOB_RECURSE HEADERS "src/_includes/*.hpp")

add_subdirectory(src)
