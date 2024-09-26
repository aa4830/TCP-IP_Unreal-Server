#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "Socket.h"
#include <WinSock2.h>
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
 
#pragma comment(lib, "ws2_32")

using json = nlohmann::json;
using namespace std;

Socket::Socket()
{
    std::srand(std::time(nullptr));

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

void Socket::SendData()
{
    string jsonString = ReadJsonFile("Data.json");
    SendJsonToClient(ClientSocket, jsonString);
}

void Socket::SendJsonToClient(int clientSocket, const std::string& jsonString)
{
    int result = send(clientSocket, jsonString.c_str(), jsonString.size(), 0);
    if (result == SOCKET_ERROR)
    {
        cout << "������ ���� ����: " << WSAGetLastError() << endl;
    }
    else
    {
        cout << "������ ���� ����, ���� ����Ʈ ��: " << result << endl;
    }
}

string Socket::ReadJsonFile(const std::string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return "{}"; // �� JSON ��ü ��ȯ
    }
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}