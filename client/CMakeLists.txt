# client/CMakeList.txt

add_executable(client src/client.cpp)

target_link_libraries(client PRIVATE common)