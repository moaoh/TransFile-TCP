#include "tcp_server.hpp"

int main() {
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  std::ofstream file("received.txt", std::ios::binary);  // 수신 파일 저장

  // socket 생성
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    std::cerr << "Socket creation error" << std::endl;
    return -1;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // socket이 특정 ip주소와 포트를 사용할 수 있도록 허용
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    std::cerr << "Bind failed" << std::endl;
    return -1;
  }

  // 소켓을 대기상태로 설정 (클라이언트의 연결을 기다리는 준비)
  if (listen(server_fd, 3) < 0) {
    std::cerr << "Listen error" << std::endl;
    return -1;
  }
  // 클라이언트의 요청을 수락
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
    std::cerr << "Accept error" << std::endl;
    return -1;
  }

  // 클라이언트로부터 받은 파일 내용을 저장
  while ((valread = read(new_socket, buffer, sizeof(buffer))) > 0) {
    file.write(buffer, valread);  // 파일에 쓰기
  }

  std::cout << "File received successfully" << std::endl;

  file.close();
  close(new_socket);
  close(server_fd);
  return 0;
}