# client/CMakeList.txt
cmake_minimum_required(VERSION 3.29)

project(client)

add_executable(client
        src/client.cpp
)

# common 헤더 파일 경로 추가
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/common)
# common 라이브러리 링크
target_link_libraries(client PRIVATE common)