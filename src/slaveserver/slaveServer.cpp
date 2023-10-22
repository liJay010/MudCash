#include "slaveServer.h"
#include "slaveService.h"
#include "json.hpp"
#include <iostream>
#include <functional>
#include <string>
#include "caclient.h"
#include "global.h"
using namespace std;
using namespace placeholders;
using json = nlohmann::json;
SlaveServer::SlaveServer(EventLoop *loop,
        const InetAddress &addr, 
        const std::string &node_name,
        std::string to_addr,
        int to_port)
    : server_(loop, addr, node_name)
        , loop_(loop),ip(to_addr),port(to_port),node_name(node_name)
{
    // 注册回调函数
    server_.setConnectionCallback(
        std::bind(&SlaveServer::onConnection, this, std::placeholders::_1)
    );

    server_.setMessageCallback(
        std::bind(&SlaveServer::onMessage, this,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    );

    // 设置合适的loop线程数量 loopthread
    server_.setThreadNum(3);
}
void SlaveServer::start()
{
    server_.start();
    //向cosever发送消息，告诉自己的ip和端口号，让cosever连接
    caclient cli(ip,port);
    int fd = cli.ca_connecting();
    
    json js;
    js["type"] = SLAVE_SEVER_CONNECT; //发送连接数据
    js["ip_port"] = node_name; //发送连接数据
    string sendBuf = js.dump();
    cli.ca_send_message(sendBuf);
    cout << cli.ca_receive_message() <<endl;
    cli.ca_closing();
}
// 连接建立或者断开的回调
void SlaveServer::onConnection(const TcpConnectionPtr &conn)
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
void SlaveServer::onMessage(const TcpConnectionPtr &conn,
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
    auto msgHandler = slaveService::instance()->getHandler(js["type"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}

std::string SlaveServer::getName(std::string tag)
{
    if(tag == "pre") return pre_node;
    else if(tag == "next") return next_node;
    else return node_name;
}
void SlaveServer::setName(std::string tag)
{
    if(tag == "pre")  pre_node = tag;
    else next_node = tag;
}