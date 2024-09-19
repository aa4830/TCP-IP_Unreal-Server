#include "Socket.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	Socket socket;
	socket.ListenForClients();
    while (true)
    {
        string command;
        cout << "명령 입력 : ";
        getline(cin, command);

        if (command == "exit")
        {
            break;
        }

        socket.SendCommandMove(command);
    }

    return 0;
}