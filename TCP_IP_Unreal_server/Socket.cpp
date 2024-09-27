#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <WinSock2.h> // Windows 소켓 라이브러리 포함
#include "Socket.h" // 사용자 정의 헤더 파일
#include "nlohmann/json.hpp" // 외부 JSON 라이브러리

#pragma comment(lib, "ws2_32")

using namespace std;
using json = nlohmann::json;

Socket::Socket() : ServerSocket(INVALID_SOCKET), ClientSocket(INVALID_SOCKET)
{
    memset(&ServerSocketAddress, 0, sizeof(ServerSocketAddress));
    memset(&ClientSocketAddress, 0, sizeof(ClientSocketAddress));

    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup 실패: " << WSAGetLastError() << endl;
        return; // 초기화 실패 시 종료
    }

    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServerSocket == INVALID_SOCKET)
    {
        cout << "소켓 생성 실패: " << WSAGetLastError() << endl;
        WSACleanup();
        return; // 소켓 생성 실패 시 종료
    }

    ServerSocketAddress.sin_family = AF_INET;
    ServerSocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerSocketAddress.sin_port = htons(10990);

    if (::bind(ServerSocket, (struct sockaddr*)&ServerSocketAddress, sizeof(ServerSocketAddress)) != 0)
    {
        cout << "bind 실패: " << WSAGetLastError() << endl;
        closesocket(ServerSocket);
        WSACleanup();
        return; // bind 실패 시 종료
    }

    if (listen(ServerSocket, 5) == SOCKET_ERROR)
    {
        cout << "listen 실패: " << WSAGetLastError() << endl;
        closesocket(ServerSocket);
        WSACleanup();
        return; // listen 실패 시 종료
    }
}


Socket::~Socket()
{
    if (ClientSocket != INVALID_SOCKET)
    {
        closesocket(ClientSocket);
    }
    if (ServerSocket != INVALID_SOCKET)
    {
        closesocket(ServerSocket);
    }
    WSACleanup();
}

void Socket::ListenForClients()
{
    int ClientSocketAddressSize = sizeof(ClientSocketAddress);
    ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientSocketAddress, &ClientSocketAddressSize);
    if (ClientSocket == INVALID_SOCKET)
    {
        cout << "클라이언트 연결 실패: " << WSAGetLastError() << endl;
        return;
    }
    else
    {
        cout << "클라이언트 연결 성공" << endl;
    }
}

string Socket::ReadJsonFile(const std::string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "파일 열기 오류: " << filename << endl;
        return "{}"; // 빈 JSON 객체 반환
    }
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

int GetRandomNumber(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void Socket::SendData()
{
    string jsonString;
    int RandomValue = GetRandomNumber(1, 5);

    switch (RandomValue)
    {
    case 1:
        jsonString = ReadJsonFile("One.json");
        break;
    case 2:
        jsonString = ReadJsonFile("Two.json");
        break;
    case 3:
        jsonString = ReadJsonFile("Three.json");
        break;
    case 4:
        jsonString = ReadJsonFile("Four.json");
        break;
    case 5:
        jsonString = ReadJsonFile("Five.json");
        break;
    default:
        cout << "Invalid value" << endl;
        return; // 유효하지 않은 경우 함수 종료
    }

    if (jsonString.empty())
    {
        cout << "전송할 데이터가 없습니다." << endl;
        return; // JSON 데이터가 비어있는 경우 함수 종료
    }

    int result = send(ClientSocket, jsonString.c_str(), static_cast<int>(jsonString.size()), 0);
    if (result == SOCKET_ERROR)
    {
        cout << "데이터 전송 실패: " << WSAGetLastError() << endl;
    }
    else 
    {
        cout << "데이터 전송 성공, 전송 바이트 수: " << result << endl;
    }
}
