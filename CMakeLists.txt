cmake_minimum_required(VERSION 3.29)
project(TranFileTCP)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)

# Thread 라이브러리
find_package(Threads REQUIRED)

# Protobuf 설정
find_package(Protobuf CONFIG REQUIRED)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# gRPC 설정
find_package(gRPC CONFIG REQUIRED)
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# 교차 컴파일 환경에서 protoc 및 gRPC 플러그인 경로 설정
if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc REQUIRED)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin REQUIRED)
endif()

# 하위 디렉터리 추가
add_subdirectory(common)

add_subdirectory(server)
add_subdirectory(client)