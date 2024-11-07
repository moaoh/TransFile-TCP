# common/CMakeList.txt
add_library(common STATIC src/network_utils.cpp)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)