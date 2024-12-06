# root/CMakeList.txt
cmake_minimum_required(VERSION 3.29)

project(TranFileTCP)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(common)

add_subdirectory(server)
add_subdirectory(client)