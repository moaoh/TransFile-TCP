#include "chat_server.hpp"

// gRPC
class ChatServiceImpl final : public chat::ChatService::Service {
public:
    grpc::Status SayHello(grpc::ServerContext* context, const chat::HelloRequest* request, chat::HelloResponse* response) override {
        std::string name = request->name();
        response->set_message("Hello, " + name + "!");
        return grpc::Status::OK;
    }
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