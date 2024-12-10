#include "chat_client.hpp"

// gRPC
void ChatClient::chatLoop(chat::ChatRoomResponse &response) {
    std::string userId = response.user_id();
    std::string roomId = response.room_id();

    std::cout << "ChatRoom ID : " << roomId << std::endl;
    std::cout << "To leave the chat room, enter ‘exit’." << std::endl;

    grpc::ClientContext messageContext;
    auto stream = _stub->Message(&messageContext);

    chat::ChatMessageRequest initMessage;
    initMessage.set_user_id(userId);
    initMessage.set_room_id(roomId);
    initMessage.set_message("");
    stream->Write(initMessage);

    bool isStreamActive = true;
    std::thread writer([&]() {
        while (isStreamActive) {
            std::string user_input;
            std::getline(std::cin, user_input);

            if (user_input == "exit" || isStreamActive == false) {
                stream->WritesDone();
                isStreamActive = false;
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
                isStreamActive = false;
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
        std::cout << "invalid chatRoom" << std::endl;
    }
}

void ChatClient::join(const std::string &userId, const std::string &roomId) {
    chat::ChatRoomRequest chatRoomRequest;
    chat::ChatRoomResponse chatRoomResponse;

    grpc::ClientContext JoinRoomContext;
    chatRoomRequest.set_room_id(roomId);
    chatRoomRequest.set_user_id(userId);
    grpc::Status status = _stub->JoinRoom(&JoinRoomContext, chatRoomRequest, &chatRoomResponse);
    if (status.ok()) {
        chatLoop(chatRoomResponse);
    }
    else {
        std::cout << "[ERROR] invalid ChatRoom ID" << std::endl;
    }
}

void	line_eof(void)
{
    if(std::cin.eof()) {
        exit(1);
    }
}

void explanation()
{
    std::cout << "[chat room]" << std::endl;
}

void chatRoom_loop() {
    ChatClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string roomId, userId;

    std::cout << "Enter your user ID: " << std::flush;
    std::getline(std::cin, userId);
    line_eof();
    std::cout << "Enter Join room ID (or press Enter to create a new room): " << std::flush;
    std::getline(std::cin, roomId);
    line_eof();
    if (roomId.empty()) {
        client.create(userId);
    }
    else {
        client.join(userId, roomId);
    }
}

int main() {
    explanation();
    chatRoom_loop();

    return 0;
}