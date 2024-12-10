#include "chat_server.hpp"

std::string generateRoomId() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return boost::uuids::to_string(uuid);
}

std::string ChatServiceImpl::createRoomId() {
    std::string roomId;

    while (true) {
        roomId = generateRoomId();
        if (_chatRoom.find(roomId) == _chatRoom.end()) {
            return roomId;
        }
    }
}

grpc::Status ChatServiceImpl::CreateRoom(grpc::ServerContext* context,
    const chat::ChatRoomRequest* request, chat::ChatRoomResponse* response) {

    std::string roomId = createRoomId();
    std::string userId = request->user_id();

    {
        std::lock_guard<std::mutex> lock(_room_mutex);
        response->set_user_id(userId);
        response->set_room_id(roomId);
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
        response->set_user_id(userId);
        response->set_room_id(roomId);
        _chatRoom[roomId].push_back(userId);
        return grpc::Status::OK;
    }
    return grpc::Status::CANCELLED;
}

grpc::Status ChatServiceImpl::Message(grpc::ServerContext* context,
    grpc::ServerReaderWriter<chat::ChatMessageResponse, chat::ChatMessageRequest>* stream) {

    chat::ChatMessageRequest request;
    chat::ChatMessageResponse response;
    std::string roomId, userId;

    // 초기 등록.
    if (stream->Read(&request)) {
        roomId = request.room_id();
        userId = request.user_id();

        {
            std::lock_guard<std::mutex> lock(_client_mutex);
            _userStreams[roomId][userId] = stream;
        }
        // 방에 있는 다른 유저들에게 알림 전송
        chat::ChatMessageResponse joinNotification;
        joinNotification.set_message("System: User " + userId + " has joined the room.");
        {
            std::lock_guard<std::mutex> lock(_client_mutex);
            for (const auto& client : _chatRoom[roomId]) {
                if (client != userId && _userStreams[roomId][client] != nullptr) {
                    _userStreams[roomId][client]->Write(joinNotification);
                }
            }
        }
    }

    while (stream->Read(&request)) {
        roomId = request.room_id();
        userId = request.user_id();

        std::string message = request.message();
        std::cout << "[" << userId << "] : " << message << std::endl;
        {
            std::lock_guard<std::mutex> lock(_client_mutex);
            if (_chatRoom.find(roomId) != _chatRoom.end()) {
                auto& chatRoom = _chatRoom[roomId];
                for (auto& client : chatRoom) {
                    if (client != userId) {
                        auto& clientStream = _userStreams[roomId][client];
                        if (clientStream != nullptr) {
                            response.set_message("[" + userId + "] : " + message);
                            clientStream->Write(response);
                        } else {
                            std::cout << "Warning: Stream for user " << client << " is not available." << std::endl;
                        }
                    }
                }
            }
        }
    }

    // 유저가 나갔으므로 정리 작업 수행
    {
        std::lock_guard<std::mutex> lock(_client_mutex);

        // 채팅방에서 유저 ID 제거
        auto& chatRoom = _chatRoom[roomId];
        chatRoom.erase(std::remove(chatRoom.begin(), chatRoom.end(), userId), chatRoom.end());

        // 알림 메시지 생성
        chat::ChatMessageResponse leaveNotification;
        leaveNotification.set_message("System: User " + userId + " has left the room.");

        // 유저가 나갔다는 알림을 채팅방의 다른 유저들에게 전송
        for (const auto& client : chatRoom) {
            if (_userStreams[roomId].find(client) != _userStreams[roomId].end()) {
                auto& clientStream = _userStreams[roomId][client];
                if (clientStream != nullptr) {
                    clientStream->Write(leaveNotification);
                }
            }
        }

        if (_userStreams.find(roomId) != _userStreams.end()) {
            _userStreams[roomId].erase(userId);

            if (_userStreams[roomId].empty()) {
                _userStreams.erase(roomId);
            }
        }
        if (chatRoom.empty()) {
            _chatRoom.erase(roomId);
        }
        std::cout << "User " << userId << " left room " << roomId << std::endl;
    }
    return grpc::Status::OK;
}

void chat_server(int port) {
    std::string server_address = "localhost:" + std::to_string(port);
    ChatServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Chat Server listening on " << server_address << std::endl;
    server->Wait();
}