cmake_minimum_required(VERSION 3.14)

project(mouse LANGUAGES CXX VERSION 1.0)

add_executable(mouse main.cpp ${INCLUDE}/mms_api.hpp ${INCLUDE}/mms_api.cpp ${INCLUDE}/FloodV2.hpp)

