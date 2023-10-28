#pragma once
#include <string>
#include "TcpServer.h"
#include "slaveService.h"

class SlaveServer
{
/*
    存储节点，将该节点使用跳表存储该节点的数据，并且是一个服务器，与协调服务器建立长期tcp连接。并通过定时器保活。
    在节点中存储有上一个节点的数据，上一个节点，以及下一个节点。当数据改变时，需要进行数据迁移。
*/
public:
    SlaveServer(EventLoop *loop,
        const InetAddress &addr, 
        const std::string &node_name,
        std::string to_addr,
        int to_port);

    void start();

private:
    // 连接建立或者断开的回调
    void onConnection(const TcpConnectionPtr &conn);

    // 可读写事件回调
    void onMessage(const TcpConnectionPtr &conn,
                Buffer *buffer,
                Timestamp time);
    EventLoop *loop_;
    TcpServer server_;

    slaveService slaveservice;
    std::string node_name; //ip:port
    std::string ip; //对端ip
    int port;//对端port
};