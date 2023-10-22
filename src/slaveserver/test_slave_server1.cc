#include <string>
#include <functional>
#include "slaveServer.h"

int main()
{
    EventLoop loop;
    InetAddress addr(6668);
    SlaveServer server(&loop, addr, "127.0.0.1:6668","127.0.0.1",8000); // Acceptor non-blocking listenfd  create bind 
    server.start(); // listen  loopthread  listenfd => acceptChannel => mainLoop =>
    loop.loop(); // 启动mainLoop的底层Poller

    return 0;
}