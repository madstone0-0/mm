cmake_minimum_required(VERSION 3.14)

project(mouse LANGUAGES CXX C VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
option(ARDUINO "Arduino" OFF)
option(ARDUINO_CMAKE "Arduino Cmake" OFF)
message("Arduino Cmake: ${ARDUINO_CMAKE}")
message("Arduino: ${ARDUINO}")
if(NOT ARDUINO AND NOT ARDUINO_CMAKE) 
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_C_FLAGS "-lncursesw")
    set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -fsanitize=address")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -fno-ident -ftree-vectorize -ffast-math -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,-s -flto")
elseif(ARDUINO_CMAKE)
    set(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake) # Arduino Toolchain
    include(cmake/Platform/Arduino.cmake)
    print_board_list()
    print_programmer_list()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -fno-ident -ftree-vectorize -ffast-math -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,-s -flto")
    set(ARDUINO_DEFAULT_BOARD uno) # Default Board ID
    set(ARDUINO_DEFAULT_PORT /dev/ttyACM0) # Default Port
elseif (ARDUINO)
    set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
    add_compile_definitions(-DARDUINO)
    set(ARDUINO_INC /home/mads/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino)
    set(ARDUINO_STL /home/mads/Arduino/libraries/ArduinoSTL/src)
    include_directories(${ARDUINO_INC} ${ARDUINO_STL} /home/mads/.arduino15/packages/arduino/hardware/avr/1.8.6/variants/mega)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(INCLUDE ${CMAKE_SOURCE_DIR}/src/include)
include_directories(${INCLUDE})
set(UTILS ${INCLUDE}/utils.hpp)
set(THIRD_PARTY ${INCLUDE}/third_party)
# file(GLOB_RECURSE HEADERS "src/_includes/*.hpp")

add_subdirectory(src)
