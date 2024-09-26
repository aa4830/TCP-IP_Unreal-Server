#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "Socket.h"
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include <string>

#pragma comment(lib, "ws2_32")

using json = nlohmann::json;
using namespace std;

Socket::Socket()
{
    std::srand(std::time(nullptr));

    std::ifstream inputFile("C:\\Work\\TCP_IP_Unreal_Server\\data.json");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open JSON file." << std::endl;
        return 1;
    }

    ServerSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&ServerSocketAddress, 0, sizeof(ServerSocketAddress));
    ServerSocketAddress.sin_family = AF_INET;
    ServerSocketAddress.sin_addr.s_addr = inet_addr("192.168.0.108");
    ServerSocketAddress.sin_port = htons(10880);

    bind(ServerSocket, (struct sockaddr*)&ServerSocketAddress, sizeof(ServerSocketAddress));

    listen(ServerSocket, 5);
}

Socket::~Socket()
{
    closesocket(ServerSocket);
    closesocket(ClientSocket);
    WSACleanup();
}

void Socket::ListenForClients()
{
    memset(&ClientSocketAddress, 0, sizeof(ClientSocketAddress));
    int ClientSocketAddressSize = sizeof(ClientSocketAddress);
    ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientSocketAddress, &ClientSocketAddressSize);
    if (ClientSocket < 0)
    {
        cout << "Ŭ���̾�Ʈ ���� ����" << endl;
        return;
    }
    else
    {
        cout << "Ŭ���̾�Ʈ ���� ����" << endl;
    }
}

void Socket::SendCommandMove(const string& command)
{
    int commandSize = static_cast<int>(command.size());
    int Result = send(ClientSocket, command.c_str(), commandSize, 0);
    if (Result != SOCKET_ERROR)
    {
        cout << "��� ����: " << command << endl;
    }
}