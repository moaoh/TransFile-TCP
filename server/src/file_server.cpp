#include "file_server.hpp"

// tcp
std::vector<int> client_sockets;
std::vector<int> waiting_for_receive;

void read_file(int send_sock, int receive_sock) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    const char *action = "READY";
    send(send_sock, action, strlen(action) + 1, 0);

    while ((bytes_received = recv(send_sock, buffer, sizeof(buffer), 0)) > 0) {
        send(receive_sock, buffer, bytes_received, 0);
    }

    std::cout << "File transfer complete." << std::endl;
}

void send_file(int client_sock) {
    std::cout << "in send_file" << std::endl;
    if (waiting_for_receive.empty()) {
        std::cout << "No clients waiting to receive the file." << std::endl;
        waiting_for_receive.push_back(client_sock);
    } else {
        int receive_sock = waiting_for_receive.back();
        waiting_for_receive.pop_back();
        read_file(client_sock, receive_sock);
    }
}

void receive_file(int client_sock) {
    std::cout << "in receive_file" << std::endl;

    if (waiting_for_receive.empty()) {
        waiting_for_receive.push_back(client_sock);
    }
    else {
        int send_sock = waiting_for_receive.back();
        waiting_for_receive.pop_back();
        read_file(send_sock, client_sock);
    }
}

void handle_client(int client_sock, const char *client_type) {
    std::cout << "in handle_client" << std::endl;
    std::cout << client_sock << std::endl;
    if (strncmp(client_type, "SEND", 4) == 0) {
        std::cout << "Client requested to send a file." << std::endl;
        send_file(client_sock);
    }
    else if (strncmp(client_type, "RECEIVE", 7) == 0) {
        std::cout << "Client receive file." << std::endl;
        receive_file(client_sock);
    }
}

void file_server_loop(int port) {
    int server_fd, client_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return ;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 3. 소켓 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return;
    }

    // 4. 연결 대기 (리스닝 상태로 전환)
    if (listen(server_fd, 5) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return;
    }

    while (true) {
        if ((client_sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "accept error." << std::endl;
            return ;
        }

        char client_type[BUFFER_SIZE];
        int bytes_received = recv(client_sock, client_type, sizeof(client_type), 0);
        if (bytes_received > 0) {
            client_sockets.push_back(client_sock);
            std::cout << "Client " << client_type << " connected." << std::endl;
        }

        // 클라이언트 요청 처리
        handle_client(client_sock, client_type);
    }

    close(server_fd);
}

int file_server(int port) {
    std::cout << "File Server listening on localhost:" << port << std::endl;
    file_server_loop(port);
    return 0;
}