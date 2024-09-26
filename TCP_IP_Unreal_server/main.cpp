#include "Socket.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

int main()
{
    Socket socket;
    socket.ListenForClients();
    while (true)
    {
        socket.SendData();
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}