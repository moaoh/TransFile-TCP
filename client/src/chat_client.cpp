#include "chat_client.hpp"

// gRPC
void ChatClient::create() {
    std::cout << "create" << std::endl;
    chat::ChatRoomRequest request;
    chat::ChatRoomResponse response;

    grpc::ClientContext createRoomContext;
    request.set_message("Hi!");
    grpc::Status status = _stub->CreateRoom(&createRoomContext, request, &response);

    if (status.ok()) {
        grpc::ClientContext messageContext;
        std::unique_ptr<grpc::ClientReaderWriter<chat::ChatMessageRequest, chat::ChatMessageResponse>> stream(
            _stub->Message(&messageContext)  // 양방향 스트리밍 RPC 호출
        );

        // 메시지 전송 스레드
        std::thread writer([&]() {
            while (true) {
                std::string user_input;
                std::cout << "Enter message: ";
                std::getline(std::cin, user_input);

                if (user_input == "exit") {
                    stream->WritesDone();
                    break;
                }

                chat::ChatMessageRequest msgRequest;
                msgRequest.set_user_id("user123");
                msgRequest.set_message(user_input);
                stream->Write(msgRequest);
            }
        });

        // 메시지 수신 스레드
        std::thread reader([&]() {
            chat::ChatMessageResponse msgResponse;
            while (stream->Read(&msgResponse)) {
                std::cout << "Received from server: " << msgResponse.message() << std::endl;
            }
        });

        writer.join();
        reader.join();

        grpc::Status status = stream->Finish();
        if (!status.ok()) {
            std::cerr << "RPC failed: " << status.error_message() << std::endl;
        }
    }
}


void ChatClient::join() {
    std::cout << "join" << std::endl;
    chat::ChatRoomRequest request;
    request.set_message("Hi!");

    chat::ChatRoomResponse response;
    grpc::ClientContext context;
    grpc::Status status = _stub->JoinRoom(&context, request, &response);

    if (status.ok()) {
        std::cout << "Server response: " << response.message() << std::endl;
    } else {
        std::cout << "RPC failed" << std::endl;
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