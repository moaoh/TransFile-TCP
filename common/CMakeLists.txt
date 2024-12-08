# common/CMakeList.txt
cmake_minimum_required(VERSION 3.29)

project(common)

# .proto 파일 경로 설정
get_filename_component(sample_proto "../protos/chat.proto" ABSOLUTE)
get_filename_component(sample_proto_path "${sample_proto}" PATH)

# 컴파일된 .proto 파일의 소스와 헤더 설정
set(sample_proto_sources "${CMAKE_CURRENT_BINARY_DIR}/chat.pb.cc")
set(sample_proto_headers "${CMAKE_CURRENT_BINARY_DIR}/chat.pb.h")
set(sample_grpc_sources "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.cc")
set(sample_grpc_headers "${CMAKE_CURRENT_BINARY_DIR}/chat.grpc.pb.h")

# .proto 파일 컴파일을 위한 custom command 설정
add_custom_command(
        OUTPUT "${sample_proto_sources}" "${sample_proto_headers}" "${sample_grpc_sources}" "${sample_grpc_headers}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}" --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${sample_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${sample_proto}"
        DEPENDS "${sample_proto}"
)

# include 디렉토리 설정
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# sample_grpc_proto 라이브러리 추가
add_library(common
        ${sample_proto_sources}
        ${sample_proto_headers}
        ${sample_grpc_sources}
        ${sample_grpc_headers}
)

# 생성된 헤더 파일 경로를 공개(include) 디렉토리로 추가
target_include_directories(common PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}  # 프로토 컴파일 결과 헤더 경로
        ${CMAKE_CURRENT_SOURCE_DIR}  # 필요한 경우 common 디렉터리 포함
)

# gRPC 및 Protobuf 라이브러리 링크
target_link_libraries(common
        ${_PROTOBUF_LIBPROTOBUF}
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
)