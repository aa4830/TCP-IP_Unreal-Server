#include "Socket.h"
#include <thread>
#include <chrono>

using namespace std;

int main()
{
    Socket socket;

    // Ŭ���̾�Ʈ�� ������ ������ ���
    socket.ListenForClients();

    // Ŭ���̾�Ʈ�� ����� �� ������ ���� ����
    while (true)
    {
        socket.SendData();  // Ŭ���̾�Ʈ�� ����� ���¿����� ������ ����
        this_thread::sleep_for(chrono::seconds(1)); // 1�� ���
    }

    return 0;
}