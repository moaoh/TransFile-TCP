#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <map>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int file_server(int port);