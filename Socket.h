#pragma once
#include <WinSock2.h>
#include <string>

class Socket
{
public:
	Socket();
	~Socket();

	void ListenForClients();
	void SendCommandMove(const std::string& command);

private:
	SOCKET ServerSocket;
	SOCKET ClientSocket;
	struct sockaddr_in ServerSocketAddress;
	struct sockaddr_in ClientSocketAddress;

};