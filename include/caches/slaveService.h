#pragma once
#include <string>
#include "TcpServer.h"
#include "Logger.h"
#include <string>
#include <functional>
#include <unordered_map>
#include <mutex>
using namespace std;
#include "json.hpp"
#include <vector>
#include "KVdatabase.h"
using json = nlohmann::json;

// 表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 聊天服务器业务类
class slaveService
{
public:
    // 处理业务
    slaveService();
    void Slave_GET(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_PUT(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_UPDATE(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_DELETE(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_BACKUP(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_BACKUP_RECV(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_BACKUP_PRE(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_NEW_SLAVE2(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_NEW_SLAVE2_RECV(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_NEW_SLAVE_UP2(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_DELETE_BK(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn){};
    // 服务器异常，业务重置方法
    void reset(){};
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);

private:
    mutex _connMutex;
    unordered_map<int, MsgHandler> _msgHandlerMap;
    KVdatabase<string,string> backup;
    KVdatabase<string,string> self_data;
};
