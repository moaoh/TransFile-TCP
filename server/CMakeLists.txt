# server/CMakeList.txt
cmake_minimum_required(VERSION 3.29)

project(server)

add_executable(fc_server
        src/fc_server.cpp
)

add_library(chat_server SHARED
        src/chat_server.cpp
)

add_library(file_server SHARED
        src/file_server.cpp
)

target_include_directories(fc_server PRIVATE
        ${CMAKE_SOURCE_DIR}/common
        ${CMAKE_SOURCE_DIR}/src
)
target_include_directories(file_server PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_include_directories(chat_server PRIVATE ${CMAKE_SOURCE_DIR}/common)

target_link_libraries(fc_server PRIVATE
        common
        file_server
        chat_server
)
target_link_libraries(file_server PRIVATE common)
target_link_libraries(chat_server PRIVATE common)

add_custom_command(TARGET fc_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fc_server> ${CMAKE_SOURCE_DIR}/bin
)