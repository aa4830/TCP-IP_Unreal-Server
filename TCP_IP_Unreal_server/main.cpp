#include "Socket.h"
#include <thread>
#include <chrono>

using namespace std;

int main()
{
    Socket socket;

    // 클라이언트를 연결할 때까지 대기
    socket.ListenForClients();

    // 클라이언트가 연결된 후 데이터 전송 시작
    while (true)
    {
        socket.SendData();  // 클라이언트가 연결된 상태에서만 데이터 전송
        this_thread::sleep_for(chrono::seconds(1)); // 1초 대기
    }

    return 0;
}