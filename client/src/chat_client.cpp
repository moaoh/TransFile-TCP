#include "chat_client.hpp"

// gRPC
void ChatClient::chatLoop(chat::ChatRoomResponse &response) {
    std::string userId = response.user_id();
    std::string roomId = response.room_id();

    std::cout << "ChatRoom ID : " << roomId << std::endl;
    std::cout << "Out of the ChatRoom : exit" << std::endl;

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

void ChatClient::create() {
    chat::ChatRoomRequest chatRoomRequest;
    chat::ChatRoomResponse chatRoomResponse;
    std::string userId;

    std::cout << "Enter your user ID: " << std::flush;;
    std::getline(std::cin, userId);
    std::cout << "your user ID: " << userId << std::endl;

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

void ChatClient::join() {
    chat::ChatRoomRequest chatRoomRequest;
    chat::ChatRoomResponse chatRoomResponse;
    std::string roomId, userId;

    std::cout << "Enter your user ID: " << std::flush;;
    std::getline(std::cin, userId);
    std::cout << "your user ID: " << userId << std::endl;

    std::cout << "Enter room id: " << std::flush;;
    std::getline(std::cin, roomId);

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
    std::cout << "[chat room]" << "\n";
    std::cout << "1 : create room." << "\n";
    std::cout << "2 : join room." << "\n";
    std::cout << "3 : program quits." << "\n";
}

int main() {
    std::string input;
    ChatClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    explanation();
    while (true) {
        std::getline(std::cin, input);
        line_eof();
        if (input == "1") {
            client.create();
            break;
        }
        else if (input == "2") {
            client.join();
            break;
        }
        else if (input == "3") {
            std::cout << "exit." << std::endl;
            break;
        }
        else {
            std::cout << "Invalid input value." << std::endl;
        }
    }
    return 0;
}
/*
채팅방을 생성.
채팅방이 생성되면 해당 채팅방에 접속할 수 있는 코드 생성.
다른 클라이언트 측에서 채팅방 코드를 입력하고 해당 채팅방코드가 일치한다면 해당 채팅방에 입장.
해당 채팅방에 들어와있는 클라이언트들과 채팅가능.
채팅방을 나갔다면 나갔다고 표시

ex

1. create room
2. join room
3. exit

1.
room : 12321a

2.
send room code :
12321a

join room
 */