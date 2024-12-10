#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <map>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <common.hpp>

int file_server(int port);

class FileServer
{
public:
  FileServer() {};

  void read_file(int send_sock, int receive_sock);
  void send_file(int client_sock);
  void receive_file(int client_sock);
  void handle_client(int client_sock, const char *client_type);
  void file_server_loop(int port);

private:
  std::vector<int> client_sockets;
  std::vector<int> waiting_for_receive;
};