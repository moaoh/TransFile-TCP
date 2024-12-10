cmake_minimum_required(VERSION 3.29)

project(client)

add_library(chat_client SHARED
        src/chat_client.cpp
)

add_library(file_client SHARED
        src/file_client.cpp
)

add_executable(fc_client
        src/fc_client.cpp
)

target_include_directories(chat_client PRIVATE
        ${CMAKE_SOURCE_DIR}/common
)
target_include_directories(file_client PRIVATE
        ${CMAKE_SOURCE_DIR}/common
)
target_include_directories(fc_client PRIVATE
        ${CMAKE_SOURCE_DIR}/common
)

target_link_libraries(chat_client PRIVATE
        common
)
target_link_libraries(file_client PRIVATE
        common
)

target_link_libraries(fc_client PRIVATE
        file_client
        chat_client
        common
)

add_custom_command(TARGET fc_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fc_client> ${CMAKE_SOURCE_DIR}/bin
)