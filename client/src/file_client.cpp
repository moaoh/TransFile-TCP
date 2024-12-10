#include "file_client.hpp"

// tcp
void send_client(int client_fd, const std::string& filePath) {
    int bytes_received;
    char buffer[BUFFER_SIZE];
    const char *action = "SEND";
    send(client_fd, action, strlen(action) + 1, 0);

    while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        if (strncmp(buffer, "READY", 5) == 0) {
            break;
        }
    }

    std::ifstream file(filePath, std::ios::binary);
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

void receive_client(int client_fd) {
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

int ready_client() {
    int client_fd;
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

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

void	line_eof(void)
{
    if(std::cin.eof()) {
        exit(1);
    }
}

void file_loop() {
    int client_fd;
    std::string type, filePath;
    std::cout << "Enter type : " << std::flush;
    std::getline(std::cin, type);
    line_eof();

    if (type == "1" || type == "SEND") {
        client_fd = ready_client();
        std::cout << "file Path : " << std::flush;
        std::getline(std::cin, filePath);
        line_eof();
        send_client(client_fd, filePath);
    }
    else if (type == "2"|| type == "RECEIVE") {
        client_fd = ready_client();
        std::cout << "waiting..." << std::endl;
        receive_client(client_fd);
    }
}

void explanation()
{
    std::cout << "[file]" << std::endl;
    std::cout << "1 : SEND" << std::endl;
    std::cout << "2 : RECEIVE" << std::endl;
}

int main() {
    explanation();
    file_loop();
    return 0;
}