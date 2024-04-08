cmake_minimum_required(VERSION 3.14)

project(mouse LANGUAGES CXX C VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS "-lncursesw")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -fsanitize=address")
# set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -fno-ident -ftree-vectorize -ffast-math -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,-s -flto")
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

#################
# PkgConfig     #
#################
find_package(PkgConfig REQUIRED)
pkg_search_module(TBB REQUIRED tbb IMPORTED_TARGET)

#################
# Catch2        #
#################
find_package(Catch2 3 REQUIRED)

#################
# Boost Library #
#################
set( Boost_USE_STATIC_LIBS ON )
set(Boost_USE_MULTITHREADED ON)
set( Boost_USE_STATIC_RUNTIME OFF )
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
# link_libraries(${Boost_LIBRARIES})

add_subdirectory(src)
