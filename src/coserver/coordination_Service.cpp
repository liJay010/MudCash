#include "coordination_Service.h"
#include "global.h"
#include "caclient.h"
#include <memory>
#include <iostream>
using namespace std;
using namespace placeholders;

// 获取单例对象的接口函数
coordination_Service *coordination_Service::instance()
{
    static coordination_Service service;
    return &service;
}
// 处理业务
void coordination_Service::clientACK(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    json res;
    
    res["type"] = CLIENT_CONNECTACK; //发送连接数据
    string sendBuf = res.dump();
    conn->send(sendBuf);

}
void coordination_Service::client_put(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string key = js["key"].get<string>();
    string node = hashring.search_by_name(key);
    shared_ptr<caclient> cli= _fdMap[node];
    string sendBuf = js.dump();
    cli->ca_send_message(sendBuf);
    conn->send(cli->ca_receive_message());
}
void coordination_Service::client_get(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string key = js["key"].get<string>();
    string node = hashring.search_by_name(key);
    shared_ptr<caclient> cli = _fdMap[node];
    string sendBuf = js.dump();
    cli->ca_send_message(sendBuf);
    conn->send(cli->ca_receive_message());
}
void coordination_Service::client_delete(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string key = js["key"].get<string>();
    string node = hashring.search_by_name(key);
    shared_ptr<caclient> cli = _fdMap[node];
    string sendBuf = js.dump();
    cli->ca_send_message(sendBuf);
    conn->send(cli->ca_receive_message());
}
void coordination_Service::client_update(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string key = js["key"].get<string>();
    string value = js["value"].get<string>();
    //TODO 
    
    string node = hashring.search_by_name(key);
    shared_ptr<caclient> cli= _fdMap[node];
    string sendBuf = js.dump();
    cli->ca_send_message(sendBuf);
    conn->send(cli->ca_receive_message());
}

void coordination_Service::slave_ACK(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string name = js["ip_port"].get<string>(); //得到服务器连接的ip端口，注册哈希环，注册客户端写入
    char *command_char=(char*)name.c_str();
    string ip=strtok(command_char,":");
    char *get_char=strtok(NULL,":");
    string port=get_char;

    shared_ptr<caclient> cli = make_shared<caclient>(ip,stoi(port));
    int fd = cli->ca_connecting(); //与服务器建立连接

    hashring.add_real_node(name,300);

    //此处进行哈希环注册
    //LOG_INFO("accept fd  %d********* %s",stoi(port),ip.c_str());
    //cout << port << " " << ip << endl;
    //TODO 
    _fdMap[name] = cli;
    json res;
    res["type"] = SLAVE_SEVER_CONNECT_ACK; //发送连接数据
    string sendBuf = res.dump();
    conn->send(sendBuf);
}
// 获取消息对应的处理器
MsgHandler coordination_Service::getHandler(int msgid)
{
    // 记录错误日志，msgid没有对应的事件处理回调
    auto it = _msgHandlerMap.find(msgid);
    if (it == _msgHandlerMap.end())
    {
        // 返回一个默认的处理器，空操作
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp) {
            cout << "msgid:" << msgid << " can not find handler!";
        };
    }
    else
    {
        return _msgHandlerMap[msgid];
    }
}
coordination_Service::coordination_Service()
{
    // 用户基本业务管理相关事件处理回调注册
    _msgHandlerMap.insert({CLIENT_CONNECT, std::bind(&coordination_Service::clientACK, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_PUT, std::bind(&coordination_Service::client_put, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_GET, std::bind(&coordination_Service::client_get, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_DELETE, std::bind(&coordination_Service::client_delete, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_UPDATE, std::bind(&coordination_Service::client_update, this, _1, _2, _3)});

    _msgHandlerMap.insert({SLAVE_SEVER_CONNECT, std::bind(&coordination_Service::slave_ACK, this, _1, _2, _3)});
    
}
