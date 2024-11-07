# client/Dockerfile
FROM transfile-tcp-common AS common

# 클라이언트 소스 복사
COPY ./ /app
WORKDIR /app

# 클라이언트 빌드
RUN rm -rf build && mkdir build && cd build && \
    cmake .. && \
    make

# 클라이언트 실행
CMD ["/app/build/client/client"]