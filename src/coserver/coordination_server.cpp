#include "coordination_server.h"
#include "coordination_Service.h"
#include "json.hpp"
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;
coordination_Server::coordination_Server(EventLoop *loop,
        const InetAddress &addr, 
        const std::string &node_name)
    : server_(loop, addr, node_name)
        , loop_(loop)
{
    // 注册回调函数
    server_.setConnectionCallback(
        std::bind(&coordination_Server::onConnection, this, std::placeholders::_1)
    );

    server_.setMessageCallback(
        std::bind(&coordination_Server::onMessage, this,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    );

    // 设置合适的loop线程数量 loopthread
    server_.setThreadNum(3);
}
void coordination_Server::start()
{
    server_.start();
    //发送已经准备好了的消息
}
// 连接建立或者断开的回调
void coordination_Server::onConnection(const TcpConnectionPtr &conn)
{
    if (conn->connected())
    {
        //LOG_INFO("Connection UP : %s", conn->peerAddress().toIpPort().c_str());
    }
    else
    {
        //LOG_INFO("Connection DOWN : %s", conn->peerAddress().toIpPort().c_str());
    }
}

// 可读写事件回调
void coordination_Server::onMessage(const TcpConnectionPtr &conn,
            Buffer *buffer,
            Timestamp time)
{
    std::string buf = buffer->retrieveAllAsString();

    // 测试，添加json打印代码
    //std::cout << buf << std::endl;

    // 数据的反序列化
    json js = json::parse(buf);
    // 达到的目的：完全解耦网络模块的代码和业务模块的代码
    // 通过js["msgid"] 获取=》业务handler=》conn  js  time
    //cout << js["type"].get<int>() <<endl;
    auto msgHandler = coordination_Service::instance()->getHandler(js["type"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}
