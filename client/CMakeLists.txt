# client/CMakeList.txt
cmake_minimum_required(VERSION 3.29)

project(client)

add_executable(chat_client
        src/chat_client.cpp
)

add_executable(file_client
        src/file_client.cpp
)

target_include_directories(chat_client PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_include_directories(file_client PRIVATE ${CMAKE_SOURCE_DIR}/common)

target_link_libraries(chat_client PRIVATE common)
target_link_libraries(file_client PRIVATE common)

add_custom_command(TARGET chat_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:chat_client> ${CMAKE_SOURCE_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:file_client> ${CMAKE_SOURCE_DIR}/bin
)