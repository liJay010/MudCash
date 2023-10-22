#include <string>
#include <functional>
#include "coordination_server.h"

int main()
{
    EventLoop loop;
    InetAddress addr(8000);
    coordination_Server server(&loop, addr, "127.0.0.1:8000"); // Acceptor non-blocking listenfd  create bind 
    server.start(); // listen  loopthread  listenfd => acceptChannel => mainLoop =>
    loop.loop(); // 启动mainLoop的底层Poller

    return 0;
}