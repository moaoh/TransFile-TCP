# server/Dockerfile
FROM transfile-tcp-common AS common

# 서버 소스 복사
COPY ./ /app
WORKDIR /app

# 서버 빌드
RUN rm -rf build && mkdir build && cd build && \
    cmake .. && \
    make

# 서버 실행
CMD ["/app/build/server/server"]