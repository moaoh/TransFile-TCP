cmake_minimum_required(VERSION 3.29)
project(TranFileTCP)

set(CMAKE_CXX_STANDARD 17)

find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

message(${Protobuf_DIR})

set(PROTO ${CMAKE_SOURCE_DIR}/proto/chat.proto)
set(PROTO_FILE_DIR ${CMAKE_SOURCE_DIR}/proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE} ${PROTO})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_FILE} ${PROTO})


add_subdirectory(common)

add_subdirectory(server)
add_subdirectory(client)