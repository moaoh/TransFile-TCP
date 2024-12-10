# cmake-build/Dockerfile
FROM alpine:latest

# 필수 패키지 설치
RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    git \
    protobuf-dev \
    boost-dev \
    clang \
    libstdc++ \
    bash \
    && rm -rf /var/cache/apk/*

# protobuf와 gRPC 소스 다운로드
WORKDIR /protobuf
RUN git clone --branch v21.12 --single-branch https://github.com/protocolbuffers/protobuf.git /protobuf
WORKDIR /grpc
RUN git clone --branch v1.68.2 --single-branch https://github.com/grpc/grpc.git /grpc

# 빌드
WORKDIR /protobuf
RUN git submodule update --init --recursive && mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make -j$(nproc) && make install

WORKDIR /grpc
RUN git submodule update --init --recursive && mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make -j$(nproc) && make install

ENV Protobuf_DIR="/usr/local/lib/cmake/protobuf"
ENV grpc_DIR="/usr/local/lib/cmake/grpc"

RUN rm -rf build && mkdir -p build && cd build && \
    cmake .. && \
    make