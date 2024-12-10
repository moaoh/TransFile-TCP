#include "chat_client.hpp"

// gRPC
void ChatClient::chatLoop(chat::ChatRoomResponse &response) {
    std::string userId = response.user_id();
    std::string roomId = response.room_id();

    std::cout << "ChatRoom ID: " << roomId << std::endl;
    std::cout << "To leave the chat room, enter ‘exit’." << std::endl;

    grpc::ClientContext messageContext;
    auto stream = _stub->Message(&messageContext);

    chat::ChatMessageRequest initMessage;
    initMessage.set_user_id(userId);
    initMessage.set_room_id(roomId);
    initMessage.set_message("");
    stream->Write(initMessage);

    std::atomic<bool> isStreamActive(true);
    std::thread writer([&]() {
        while (isStreamActive) {
            std::string user_input;
            std::getline(std::cin, user_input);

            if (user_input == "exit" || !isStreamActive.load()) {
                stream->WritesDone();
                isStreamActive.store(false);
                break;
            }

            if (isStreamActive) {
                chat::ChatMessageRequest msgRequest;
                msgRequest.set_user_id(userId);
                msgRequest.set_room_id(roomId);
                msgRequest.set_message(user_input);
                stream->Write(msgRequest);
            }
        }
    });

    std::thread reader([&]() {
        chat::ChatMessageResponse msgResponse;
        while (isStreamActive) {
            if (!stream->Read(&msgResponse)) {
                std::cerr << "Connection closed or error occurred. Exiting..." << std::endl;
                isStreamActive.store(false);
                break;
            }
            std::cout << msgResponse.message() << std::endl;
        }
    });

    writer.join();
    reader.join();

    grpc::Status status = stream->Finish();
    if (!status.ok()) {
        std::cerr << "Stream finished with error: " << status.error_message() << std::endl;
    }
}

void ChatClient::create(const std::string &userId) {
    chat::ChatRoomRequest chatRoomRequest;
    chat::ChatRoomResponse chatRoomResponse;

    grpc::ClientContext createRoomContext;
    chatRoomRequest.set_user_id(userId);
    grpc::Status status = _stub->CreateRoom(&createRoomContext, chatRoomRequest, &chatRoomResponse);
    if (status.ok()) {
        chatLoop(chatRoomResponse);
    }
    else {
        std::cerr << "[ERROR] Unable to create chat room" << std::endl;
    }
}

void ChatClient::join(const std::string &userId, const std::string &roomId) {
    chat::ChatRoomRequest chatRoomRequest;
    chat::ChatRoomResponse chatRoomResponse;

    grpc::ClientContext joinRoomContext;
    chatRoomRequest.set_room_id(roomId);
    chatRoomRequest.set_user_id(userId);
    grpc::Status status = _stub->JoinRoom(&joinRoomContext, chatRoomRequest, &chatRoomResponse);
    if (status.ok()) {
        std::cout << "Joined ChatRoom." << std::endl;
        chatLoop(chatRoomResponse);
    }
    else {
        std::cerr << "[ERROR] Invalid ChatRoom ID" << std::endl;
    }
}

void explanation()
{
    std::cout << "[chat room]" << std::endl;
}

void chatRoomLoop() {
    int chat_client_port = CHAT_PORT;
    ChatClient client(grpc::CreateChannel("localhost:" + std::to_string(chat_client_port), grpc::InsecureChannelCredentials()));
    std::string roomId, userId;

    std::cout << "Enter your User ID: ";
    std::getline(std::cin, userId);
    line_eof();

    std::cout << "Enter Room ID to Join (or press Enter to create a new room): ";
    std::getline(std::cin, roomId);
    line_eof();

    if (roomId.empty()) {
        client.create(userId);
    } else {
        client.join(userId, roomId);
    }
}

int chat_client() {
    explanation();
    chatRoomLoop();

    return 0;
}