cmake_minimum_required(VERSION 3.5.0)
project(crc VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist) # 可执行文件
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist) # 动态库
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist) # 静态库

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_FILES "src/*.cpp")
file(GLOB TEST_FILES "tests/*.cpp")

add_library(crc_static STATIC ${SRC_FILES})
add_library(crc_shared SHARED ${SRC_FILES})

set_target_properties(crc_static PROPERTIES OUTPUT_NAME "crc")
set_target_properties(crc_shared PROPERTIES OUTPUT_NAME "crc")

add_executable(tests ${TEST_FILES})
target_link_libraries(tests crc_static)
