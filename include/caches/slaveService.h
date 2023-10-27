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
#include "SkipList.h"
using json = nlohmann::json;

// 表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 聊天服务器业务类
class slaveService
{
public:
    // 获取单例对象的接口函数
    static slaveService *instance();
    // 处理业务
    void Slave_GET(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_PUT(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_UPDATE(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void Slave_DELETE(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn){};
    // 服务器异常，业务重置方法
    void reset(){};
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);

private:
    slaveService();
    mutex _connMutex;
    unordered_map<int, MsgHandler> _msgHandlerMap;
    SkipList<string,string> skiplist;
    vector<string> backup;
};
