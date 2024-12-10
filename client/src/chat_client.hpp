#pragma once

#include <iostream>
#include <thread>

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"  // chat.proto에서 생성된 헤더 파일

#include <common.hpp>

class ChatClient
{
public:
    ChatClient(std::shared_ptr<grpc::Channel> channel)
        : _stub(chat::ChatService::NewStub(channel)) {}

    void create(const std::string &userId);
    void join(const std::string &userId, const std::string &roomId);
private:
    std::unique_ptr<chat::ChatService::Stub> _stub;

    void chatLoop(chat::ChatRoomResponse &response);
};

void explanation();
void chatRoomLoop();
int chat_client();