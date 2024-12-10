#include "fc_client.hpp"

void print_client_info() {
    std::cout << "[client]" << std::endl;
}

void run_client() {
    std::string sPort;
    int port;
    int file_client_port = FILE_PORT;
    int chat_client_port = CHAT_PORT;

    std::cout << "Enter Port: " << std::flush;
    std::getline(std::cin, sPort);

    try {
        port = std::stoi(sPort);
        std::cout << "Port: " << port << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: The input is not a valid number." << std::endl;
        port = 0;
    }

    if (port == file_client_port) {
        file_client();
    }
    else if (port == chat_client_port) {
        chat_client();
    }
    else {
        std::cout << "Invalid port." << std::endl;
    }
}

int main() {
    print_client_info();
    run_client();

    return 0;
}