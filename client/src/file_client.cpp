#include "file_client.hpp"

// tcp
int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    std::ifstream file("test.txt", std::ios::binary);  // 텍스트 파일 열기

    if (!file.is_open()) {
        std::cerr << "File open error!" << std::endl;
        return -1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // "127.0.0.1" (문자열) → 0x7F000001 (이진 형식, 네트워크 바이트 순서)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported" << std::endl;
        return -1;
    }

    // server 연결요청
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    // 파일 내용을 서버로 전송
    while (file.read(buffer, sizeof(buffer))) {
        send(sock, buffer, file.gcount(), 0);  // 파일 내용을 서버로 전송
    }

    // 나머지 내용 전송
    if (file.gcount() > 0) {
        send(sock, buffer, file.gcount(), 0);
    }

    std::cout << "File sent successfully" << std::endl;

    file.close();
    close(sock);
    return 0;
}