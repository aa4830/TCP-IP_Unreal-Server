#pragma once
#include <WinSock2.h>
#include <string>

class Socket
{
public:
	Socket();
	~Socket();

	void ListenForClients();
	void SendData();
	void SendJsonToClient(int clientSocket, const std::string& jsonString);
	std::string ReadJsonFile(const std::string& filename);

private:
	SOCKET ServerSocket;
	SOCKET ClientSocket;
	struct sockaddr_in ServerSocketAddress;
	struct sockaddr_in ClientSocketAddress;

};