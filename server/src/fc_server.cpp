#include "fc_server.hpp"

void startFileServer(int port) {
    file_server(port);
}

void startChatServer(int port) {
    chat_server(port);
}

int main() {
    // 두 서버를 서로 다른 포트에서 실행
    int fileServerPort = 12345;
    int chatServerPort = 50051;

    // 파일 서버를 별도의 스레드에서 실행
    std::thread fileServerThread([&]() {
        startFileServer(fileServerPort);
    });

    // gRPC 서버 실행
    startChatServer(chatServerPort);

    fileServerThread.join();
    return 0;
}