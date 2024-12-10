#include "file_client.hpp"

// 파일 전송
void send_file(int client_fd, const std::string& file_path) {
    int bytes_received;
    char buffer[BUFFER_SIZE];
    const char *action = "SEND";
    send(client_fd, action, strlen(action) + 1, 0);

    while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        if (strncmp(buffer, "READY", 5) == 0) {
            break;
        }
    }

    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "File open error!" << std::endl;
        return ;
    }

    while (file.read(buffer, sizeof(buffer))) {
        send(client_fd, buffer, file.gcount(), 0);
    }
    if (file.gcount() > 0) {
        send(client_fd, buffer, file.gcount(), 0);
    }

    const char *end_signal = "END_OF_FILE";
    send(client_fd, end_signal, strlen(end_signal) + 1, 0);

    file.close();
    close(client_fd);
}

// 파일 수신
void receive_file(int client_fd) {
    int bytes_received;
    const char *action = "RECEIVE";
    send(client_fd, action, strlen(action) + 1, 0);

    std::ofstream file("received.txt", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return;
    }

    char buffer[BUFFER_SIZE];
    std::string data_accumulator;
    while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        data_accumulator.append(buffer, bytes_received);

        size_t end_signal_pos = data_accumulator.find("END_OF_FILE");
        if (end_signal_pos != std::string::npos) {
            file.write(data_accumulator.data(), end_signal_pos);
            std::cout << "End of file signal received." << std::endl;
            break;
        }
        file.write(data_accumulator.data(), data_accumulator.size());
        data_accumulator.clear();
    }

    file.close();
    close(client_fd);
}

// 클라이언트 소켓 생성
int create_client_socket() {
    int client_fd;
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(FILE_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported" << std::endl;
        return -1;
    }

    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    return client_fd;
}

// 파일 전송/수신 루프
void file_transfer_loop() {
    int client_fd;
    std::string type, file_path;
    std::cout << "Enter type (SEND/RECEIVE): " << std::flush;
    std::getline(std::cin, type);
    line_eof();

    if (type == "SEND") {
        client_fd = create_client_socket();
        std::cout << "File path: " << std::flush;
        std::getline(std::cin, file_path);
        line_eof();
        send_file(client_fd, file_path);
    }
    else if (type == "RECEIVE") {
        client_fd = create_client_socket();
        std::cout << "Waiting for file..." << std::endl;
        receive_file(client_fd);
    }
}

// 클라이언트 설명 출력
void print_client_info() {
    std::cout << "[File delivery]" << std::endl;
    std::cout << "SEND: Send a file" << std::endl;
    std::cout << "RECEIVE: Receive a file" << std::endl;
}

// 파일 클라이언트 실행
int file_client() {
    print_client_info();
    file_transfer_loop();
    return 0;
}