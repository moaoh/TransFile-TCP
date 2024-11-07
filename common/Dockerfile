# common/Dockerfile
FROM alpine:latest

# 공통 빌드 툴 설치
RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    g++ \
    && rm -rf /var/cache/apk/*

# 작업 디렉토리 설정
WORKDIR /app

# common 디렉토리 복사
COPY . /app

# CMakeCache.txt 파일이 이미 있을 수 있으므로 삭제 후 다시 빌드
RUN rm -rf build && mkdir -p build && cd build && \
    cmake .. && \
    make

# CMD는 필요 없지만, 라이브러리 설치 후 common 빌드 완료