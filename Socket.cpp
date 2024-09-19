#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "Socket.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32")

using namespace std;

Socket::Socket() : ServerSocket(INVALID_SOCKET), ClientSocket(INVALID_SOCKET)
{
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
        cout<< "클라이언트 연결 실패" << endl;
        return;
    }
    else 
    {
        cout << "클라이언트 연결 성공" << endl;
    }
}

void Socket::SendCommandMove(const string& command)
{
    int commandSize = static_cast<int>(command.size());
    int Result = send(ClientSocket, command.c_str(), commandSize, 0);
    if (Result != SOCKET_ERROR)
    {
        cout << "명령 전송: " << command << endl;
    }
}