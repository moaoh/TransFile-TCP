# cmake-build/Dockerfile
FROM alpine:latest AS cmake-build

# 필수 패키지 설치
# 필요한 패키지 설치
RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    git \
    protobuf-dev \
    boost-dev \
    clang \
    libstdc++ \
    bash \
    openssl-dev \
    zlib-dev \
    g++ \
    make \
    && rm -rf /var/cache/apk/*

WORKDIR /grpc
RUN git clone --branch v1.68.2 --single-branch https://github.com/grpc/grpc.git /grpc \
    && cd /grpc \
    && git submodule update --init --recursive

WORKDIR /grpc
RUN mkdir build \
    && cd build \
    && cmake .. \
    && make -j4

WORKDIR /protobuf
RUN git clone --branch v21.12 --single-branch https://github.com/protocolbuffers/protobuf.git /protobuf \
    && cd /protobuf \
    && git submodule update --init --recursive

WORKDIR /protobuf
RUN mkdir build \
    && cd build \
    && cmake .. \
    && make -j4

FROM cmake-build

RUN rm -rf build && mkdir -p build && cd build && \
    cmake .. && \
    make