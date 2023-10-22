#pragma once
#include <string>
#include "TcpServer.h"


class coordination_Server
{
public:
    coordination_Server(EventLoop *loop,
        const InetAddress &addr, 
        const std::string &node_name);

    void start();

private:
    // 连接建立或者断开的回调
    void onConnection(const TcpConnectionPtr &conn);

    // 可读写事件回调
    void onMessage(const TcpConnectionPtr &conn,
                Buffer *buffer,
                Timestamp time);

    std::string getName(std::string tag);
    void setName(std::string tag);

    EventLoop *loop_;
    TcpServer server_;

    
};