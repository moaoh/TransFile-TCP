#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common.hpp"

#define PORT 8080

int file_server();