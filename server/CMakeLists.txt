cmake_minimum_required(VERSION 3.10)

project(Server)

# 서버 빌드
add_executable(server src/server.cpp)

# common 헤더 파일 경로 추가
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/common)

# common 라이브러리 링크
target_link_libraries(server PRIVATE common)