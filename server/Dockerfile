# server/Dockerfile
FROM cmake-build

# 서버 소스 복사
COPY ./ /app
WORKDIR /app

# 서버 실행
CMD ["/app/build/server/server"]