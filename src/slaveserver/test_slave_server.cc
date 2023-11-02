#include <string>
#include <functional>
#include "slaveServer.h"
#include <string>

int main(int argc,char *argv[])
{
    //coserverip  coserverport thisip thisport
    std::string coserverip(argv[1]),coserverport(argv[2]),thisip(argv[3]),thisport(argv[4]);
    EventLoop loop;
    InetAddress addr(stoi(thisport));
    SlaveServer server(&loop, addr, thisip+":"+thisport,coserverip,stoi(coserverport)); // Acceptor non-blocking listenfd  create bind 
    server.start(); // listen  loopthread  listenfd => acceptChannel => mainLoop =>
    loop.loop(); // 启动mainLoop的底层Poller

    return 0;
}