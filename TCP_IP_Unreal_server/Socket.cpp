#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <WinSock2.h> // Windows ���� ���̺귯�� ����
#include "Socket.h" // ����� ���� ��� ����
#include "nlohmann/json.hpp" // �ܺ� JSON ���̺귯��

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
        cout << "WSAStartup ����: " << WSAGetLastError() << endl;
        return; // �ʱ�ȭ ���� �� ����
    }

    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServerSocket == INVALID_SOCKET)
    {
        cout << "���� ���� ����: " << WSAGetLastError() << endl;
        WSACleanup();
        return; // ���� ���� ���� �� ����
    }

    ServerSocketAddress.sin_family = AF_INET;
    ServerSocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerSocketAddress.sin_port = htons(10990);

    if (::bind(ServerSocket, (struct sockaddr*)&ServerSocketAddress, sizeof(ServerSocketAddress)) != 0)
    {
        cout << "bind ����: " << WSAGetLastError() << endl;
        closesocket(ServerSocket);
        WSACleanup();
        return; // bind ���� �� ����
    }

    if (listen(ServerSocket, 5) == SOCKET_ERROR)
    {
        cout << "listen ����: " << WSAGetLastError() << endl;
        closesocket(ServerSocket);
        WSACleanup();
        return; // listen ���� �� ����
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
        cout << "Ŭ���̾�Ʈ ���� ����: " << WSAGetLastError() << endl;
        return;
    }
    else
    {
        cout << "Ŭ���̾�Ʈ ���� ����" << endl;
    }
}

string Socket::ReadJsonFile(const std::string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "���� ���� ����: " << filename << endl;
        return "{}"; // �� JSON ��ü ��ȯ
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
        return; // ��ȿ���� ���� ��� �Լ� ����
    }

    if (jsonString.empty())
    {
        cout << "������ �����Ͱ� �����ϴ�." << endl;
        return; // JSON �����Ͱ� ����ִ� ��� �Լ� ����
    }

    int result = send(ClientSocket, jsonString.c_str(), static_cast<int>(jsonString.size()), 0);
    if (result == SOCKET_ERROR)
    {
        cout << "������ ���� ����: " << WSAGetLastError() << endl;
    }
    else 
    {
        cout << "������ ���� ����, ���� ����Ʈ ��: " << result << endl;
    }
}
