#pragma once
#include <string>
#include "TcpServer.h"
#include "Logger.h"
#include <string>
#include <functional>
#include <unordered_map>
#include <mutex>
#include "consistent_hash.h"
using namespace std;
#include "json.hpp"
#include "caclient.h"
#include <memory>
using json = nlohmann::json;

// 表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 聊天服务器业务类
class coordination_Service
{
public:
    // 获取单例对象的接口函数
    static coordination_Service *instance();
    // 处理业务
    void clientACK(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void client_put(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void client_get(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void client_delete(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void client_update(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void slave_ACK(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn){};
    // 服务器异常，业务重置方法
    void reset(){};
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);

private:
    coordination_Service();
    mutex _connMutex;
    unordered_map<int, MsgHandler> _msgHandlerMap;

    unordered_map<string,shared_ptr<caclient>> _fdMap;
    consistent_hash hashring;
};
