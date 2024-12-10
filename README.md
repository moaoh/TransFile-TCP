# gRPC TCP Chat

이 프로젝트는 `gRPC`와 `TCP` 소켓을 활용하여 실시간으로 메시지를 주고받을 수 있는 채팅 애플리케이션입니다. `gRPC`를 통해 클라이언트와 서버 간의 효율적인 채팅기능을 구현하고, `TCP` 소켓을 사용하여 파일 전송 기능을 지원합니다.

## Table Of Contents

1. [Preview](#preview)
2. [Function](#function)
3. [stack](#stack)
4. [Build and Run](#build-and-run)
5. [How to use](#how-to-use)
6. [Key Features](#key-features)

## Preview

<table>
  <th colspan="2">
    chat room
  </th>
  <tr>
    <td>
      <img width="450" alt="image" src="https://github.com/user-attachments/assets/8a5ea17c-e223-4e97-a263-2f5f6f04a4f3">
    </td>
    <td>
      <img width="450" alt="image" src="https://github.com/user-attachments/assets/ad3338f5-6961-4e04-860a-ba5aba64fd90">
    </td>
  </tr>
  </table>

<table>
  <th colspan="2">
    file send & receive
  </th>
  <tr>
    <td>
      <img width="450" alt="image" src="https://github.com/user-attachments/assets/26b15134-f950-4c89-b1cc-3227447eb5f3">
    </td>
  </tr>
</table>


## Function

- **멀티 클라이언트 지원**: 여러 사용자가 동시에 채팅방에 접속하여 실시간으로 메시지를 주고받을 수 있습니다. 각 클라이언트는 독립적으로 채팅에 참여하며, 서버는 이를 효율적으로 관리합니다.
- **gRPC 기반 메시징**: `gRPC`를 사용하여 고성능의 채팅기능을 구현했습니다. `Protobuf`를 이용한 데이터 직렬화로 효율적인 네트워크 통신이 가능합니다.
- **TCP 소켓을 통한 파일 전송**: `TCP` 소켓을 이용하여 클라이언트 간 파일 전송이 가능합니다. 서버는 서버중계방식을 사용하여, 안정적인 데이터 전송을 보장합니다.

## stack

- **Language**: C++
- **Protocol**: gRPC, TCP
- **Build Tool**: CMake
- **Library**: gRPC, Protobuf, Boost
- **환경**: Docker (구성 중)

## Build and Run

1. **의존성 설치**  
   이 프로젝트는 `gRPC`, `Protobuf`, `Boost` 라이브러리를 사용합니다. 아래 명령어로 설치할 수 있습니다 (MAC):

   ```bash
   # gRPC와 Protobuf 설치
   brew install grpc
   brew install protobuf
   brew install Boost
   ```

2. 빌드 디렉토리 생성 및 프로젝트 빌드:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

3. 실행:

   두 개의 `bash`창을 열고 각각 서버와 클라이언트를 실행합니다:
   
   **server 실행**
      ```bash
      cd ../bin
      ./server
      ```
   
   **client 실행**
      ```bash
      cd ../bin
      ./client
      ```

## How to use

1. **서버 실행**
   
   먼저 서버를 실행해야 합니다. 서버는 클라이언트 요청을 처리하고 메시지를 전달합니다.

   서버는 두 가지 모드로 실행됩니다: `Chat Server`와 `File Server`입니다. 이 서버들은 각각 특정 포트에서 클라이언트의 연결을 기다립니다.

   서버를 실행할 때, 두 가지 포트를 설정할 수 있습니다. `Chat Server`는 `gRPC`를 사용하여 메시지를 주고받고, `File Server`는 `TCP`를 사용하여 파일을 전송합니다.

   ```bash
   ./fc_server
   File Server listening on localhost:12345
   Chat Server listening on localhost:50051
   ```

2. **클라이언트 실행**

   클라이언트 실행 시, 서버와 연결하기 위한 포트를 입력해야 합니다. 사용자는 `Chat Server`와 `File Server` 각각의 포트를 입력할 수 있으며, 해당 포트에서 실행되는 서버와 연결됩니다.
   
   ```bash
   ./fc_client
   [client]
   Enter Port:
   ```
   위의 메시지가 나타나면, 클라이언트는 사용자가 입력한 포트에 연결됩니다. 
   예를 들어, `Chat Server`의 포트인 `50051`을 입력하면, 클라이언트는 해당 포트에 연결된 서버와 채팅을 시작합니다.

   #### **사용 예시:**

   ```
   ./fc_client
   [client]
   Enter Port: 50051
   [chat room]
   Enter your User ID:
   ```
   그 후 클라이언트는 해당 포트에서 실행 중인 서버에 연결되어, 채팅방을 생성하거나 채팅방에 입장하는 기능을 제공합니다.
   포트를 통해 서버와 클라이언트 간의 실시간 통신이 가능하며, 서버와 클라이언트는 연결된 포트에 맞게 데이터를 주고받습니다.

## Key Features

- **멀티 클라이언트 지원**: 여러 사용자가 동시에 채팅하거나 파일을 전송할 수 있습니다.
- **gRPC 기반 채팅**: 효율적인 메시지 송수신을 위한 `gRPC`를 사용한 채팅 기능.
- **TCP 파일 전송**: `TCP` 프로토콜을 통해 파일을 전송할 수 있습니다.
- **서버 포트 선택**: 클라이언트는 연결할 서버의 포트를 지정하여 채팅 또는 파일 전송을 선택적으로 수행할 수 있습니다.
