# common/CMakeList.txt
cmake_minimum_required(VERSION 3.29)

project(common)

add_library(common STATIC
        src/network_utils.cpp
)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)