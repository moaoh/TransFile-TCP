# client/Dockerfile
FROM cmake-build

# 클라이언트 소스 복사
COPY ./ /app
WORKDIR /app

# 클라이언트 실행
CMD ["/app/build/client/client"]