#pragma once

#include <iostream>
#include <thread>

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"  // chat.proto에서 생성된 헤더 파일

class ChatClient
{
public:
    ChatClient(std::shared_ptr<grpc::Channel> channel)
        : _stub(chat::ChatService::NewStub(channel)) {}

    void create();
    void join();
private:
    std::unique_ptr<chat::ChatService::Stub> _stub;

    void chatLoop(chat::ChatRoomResponse &response);
};

int chat_client();