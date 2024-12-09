#include "chat_server.hpp"

std::string generateRandomString(size_t length) {
    const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;  // 랜덤 시드 생성
    std::mt19937 generator(rd());  // Mersenne Twister 엔진
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    std::string randomString;
    for (size_t i = 0; i < length; ++i) {
        randomString += characters[distribution(generator)];
    }

    return randomString;
}

grpc::Status ChatServiceImpl::CreateRoom(grpc::ServerContext* context,
    const chat::ChatRoomRequest* request, chat::ChatRoomResponse* response) {

    std::string roomId = generateRandomString(5);
    std::string userId = request->user_id();

    response->set_message("create room!!");
    {
        std::lock_guard<std::mutex> lock(_room_mutex);
        response->set_room_id(roomId);
        _chatRoom[roomId];
        _chatRoom[roomId].push_back(userId);
    }
    return grpc::Status::OK;
}

grpc::Status ChatServiceImpl::JoinRoom(grpc::ServerContext* context,
    const chat::ChatRoomRequest* request, chat::ChatRoomResponse* response) {

    std::string roomId = request->room_id();
    std::string userId = request->user_id();

    if (_chatRoom.find(roomId) != _chatRoom.end()) {
        std::lock_guard<std::mutex> lock(_room_mutex);
        response->set_message("join room!!");
        response->set_room_id(roomId);

        _chatRoom[roomId].push_back(userId);
        return grpc::Status::OK;
    }
    return grpc::Status::CANCELLED;
}

bool ChatServiceImpl::isUserInRoom(const std::string& roomId, const std::string& userId) {
    // 방이 존재하는지 확인
    if (_chatRoom.find(roomId) != _chatRoom.end()) {
        // 해당 방의 유저 리스트
        const std::vector<std::string>& users = _chatRoom[roomId];

        // std::find로 userId가 vector에 있는지 확인
        auto it = std::find(users.begin(), users.end(), userId);

        // 찾으면 true, 없으면 false 반환
        return it != users.end();
    }

    return false; // 방이 없으면 false 반환
}

grpc::Status ChatServiceImpl::Message(grpc::ServerContext* context,
    grpc::ServerReaderWriter<chat::ChatMessageResponse, chat::ChatMessageRequest>* stream) {

    chat::ChatMessageRequest request;
    chat::ChatMessageResponse response;

    while (stream->Read(&request)) {
        std::string roomId = request.room_id();
        std::string userId = request.user_id();

        {
            std::lock_guard<std::mutex> lock(_client_mutex);
            // 이미 스트림이 저장되어 있지 않으면 추가
            if (_userStreams[roomId].find(userId) == _userStreams[roomId].end()) {
                _userStreams[roomId][userId] = stream;  // 유저 스트림을 저장
            }
        }

        std::string message = request.message();
        std::cout << userId << ": " << message << std::endl;
        {
            std::lock_guard<std::mutex> lock(_client_mutex);
            if (_chatRoom.find(roomId) != _chatRoom.end()) {
                auto& chatRoom = _chatRoom[roomId];
                for (auto& client : chatRoom) {
                    if (client != userId) {
                        auto& clientStream = _userStreams[roomId][client];
                        if (clientStream != nullptr) {
                            response.set_message(userId + ": " + message);
                            clientStream->Write(response);
                        } else {
                            std::cout << "Warning: Stream for user " << client << " is not available." << std::endl;
                        }
                    }
                }
            }
        }
    }

    return grpc::Status::OK;
}

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