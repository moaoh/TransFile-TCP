#pragma once

#include <memory>
#include <iostream>
#include <random>

#include "chat.grpc.pb.h"
#include <grpc++/grpc++.h>

// gRPC
class ChatServiceImpl final : public chat::ChatService::Service
{
public:
    grpc::Status CreateRoom(grpc::ServerContext* context,
        const chat::ChatRoomRequest* request, chat::ChatRoomResponse* response) override;
    grpc::Status JoinRoom(grpc::ServerContext* context,
        const chat::ChatRoomRequest* request, chat::ChatRoomResponse* response) override;
    grpc::Status Message(grpc::ServerContext* context,
        grpc::ServerReaderWriter<chat::ChatMessageResponse, chat::ChatMessageRequest>* stream) override;


private:
    std::unordered_map<std::string, std::vector<std::string>> _chatRoom;
    std::unordered_map<std::string,  // room_id
        std::unordered_map<std::string, // user_id
            grpc::ServerReaderWriter<chat::ChatMessageResponse, chat::ChatMessageRequest>*>> _userStreams;
    std::mutex _room_mutex;
    std::mutex _client_mutex;

    bool isUserInRoom(const std::string& roomId, const std::string& userId);
};

void chat_server();