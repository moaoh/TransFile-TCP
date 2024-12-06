# cmake-build/Dockerfile
FROM alpine:latest

RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    g++ \
    && rm -rf /var/cache/apk/*

WORKDIR /app

COPY . .

RUN rm -rf build && mkdir -p build && cd build && \
    cmake .. && \
    make