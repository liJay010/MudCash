#include "slaveService.h"
#include "global.h"
#include "caclient.h"
using namespace std;
using namespace placeholders;

// 获取单例对象的接口函数
slaveService *slaveService::instance()
{
    static slaveService service;
    return &service;
}


// 获取消息对应的处理器
MsgHandler slaveService::getHandler(int msgid)
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
void slaveService::Slave_GET(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string key = js["key"].get<string>();
    //TODO 
    string value;
    int f = skiplist.find_element(key,&value);
    json res;
    res["type"] = SLAVE_SEVER_GET_ACK; //发送连接数据
    if(f == 0)
    {
        res["value"] = value;
        res["code"] = 0;
    }
    else res["code"] = -1;
    
    string sendBuf = res.dump();
    conn->send(sendBuf);
}
void slaveService::Slave_PUT(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string key = js["key"].get<string>();
    string value = js["value"].get<string>();
    bool bk = js["back_up"].get<bool>();
    //TODO 
    //cout << "put --------------------"<<endl;
    if(bk) backup.push_back(key); //备份key

    skiplist.insert_element(key,value);
    json res;
    res["type"] = SLAVE_SEVER_PUT_ACK; //发送连接数据
    res["code"] = 0;
    string sendBuf = res.dump();
    conn->send(sendBuf);
}
void slaveService::Slave_UPDATE(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string key = js["key"].get<string>();
    string value = js["value"].get<string>();
    //TODO 
    skiplist.insert_element(key,value);
    json res;
    res["type"] = SLAVE_SEVER_UPDATE_ACK; //发送连接数据
    res["code"] = 0;
    string sendBuf = res.dump();
    conn->send(sendBuf);
}
void slaveService::Slave_DELETE(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string key = js["key"].get<string>();
    //TODO 
    skiplist.delete_element(key);
    json res;
    res["type"] = SLAVE_SEVER_DELETE_ACK; //发送连接数据
    res["code"] = 0;
    string sendBuf = res.dump();
    conn->send(sendBuf);
}

slaveService::slaveService():skiplist(10)
{
    // 用户基本业务管理相关事件处理回调注册
    _msgHandlerMap.insert({CLIENT_GET, std::bind(&slaveService::Slave_GET, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_PUT, std::bind(&slaveService::Slave_PUT, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_UPDATE, std::bind(&slaveService::Slave_UPDATE, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_DELETE, std::bind(&slaveService::Slave_DELETE, this, _1, _2, _3)});
}