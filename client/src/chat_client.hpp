#pragma once

#include <iostream>

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"  // chat.proto에서 생성된 헤더 파일

int chat_client();