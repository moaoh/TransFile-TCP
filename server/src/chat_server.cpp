#include "chat_server.hpp"

// gRPC
class ChatServiceImpl final : public chat::ChatService::Service
{
public:
    grpc::Status CreateRoom(grpc::ServerContext* context,
        const chat::ChatRoomRequest* request, chat::ChatRoomResponse* response) override {
        std::string message = request->message();
        std::cout << message << std::endl;
        response->set_message("create room!!");
        return grpc::Status::OK;
    }

    grpc::Status JoinRoom(grpc::ServerContext* context,
        const chat::ChatRoomRequest* request, chat::ChatRoomResponse* response) override {
        std::string message = request->message();
        std::cout << message << std::endl;
        response->set_message("join room!!");
        return grpc::Status::OK;
    }

    grpc::Status Message(grpc::ServerContext* context,
        grpc::ServerReaderWriter<chat::ChatMessageResponse, chat::ChatMessageRequest>* stream) override {
        {
            std::lock_guard<std::mutex> lock(_client_mutex);
            _clients.push_back(stream);
        }
        chat::ChatMessageRequest request;
        chat::ChatMessageResponse response;

        while (stream->Read(&request)) {
            std::cout << "Received from client: " << request.message() << std::endl;

            // 다른 클라이언트들에게 메시지 브로드캐스트
            {
                std::lock_guard<std::mutex> lock(_client_mutex);
                for (auto client : _clients) {
                    if (client != stream) {  // 자기 자신 제외
                        response.set_message(request.user_id() + ": " + request.message());
                        client->Write(response);  // 다른 클라이언트에게 메시지 전송
                    }
                }
            }
        }

        // 클라이언트 스트림 제거
        {
            std::lock_guard<std::mutex> lock(_client_mutex);
            _clients.erase(std::remove(_clients.begin(), _clients.end(), stream), _clients.end());
        }

        return grpc::Status::OK;
    }
private:
    std::vector<grpc::ServerReaderWriter<chat::ChatMessageResponse, chat::ChatMessageRequest>*> _clients;
    std::mutex _client_mutex;
};

void chat_server() {
    std::string server_address("0.0.0.0:50051");
    ChatServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}