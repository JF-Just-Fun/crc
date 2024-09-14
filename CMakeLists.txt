cmake_minimum_required(VERSION 3.5.0)
project(crc VERSION 0.1.0 LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

add_library(crc_static STATIC main.cpp)
add_library(crc_shared SHARED main.cpp)

add_executable(crc_main main.cpp)
