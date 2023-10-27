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
    bool find = self_data.check_K(key) || backup.check_K(key);
    if(self_data.check_K(key)) value = self_data.find_K(key);
    if(backup.check_K(key)) value = backup.find_K(key);

    json res;
    res["type"] = SLAVE_SEVER_GET_ACK; //发送连接数据
    if(find)
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
    cout << " recv put key:" << key << " value: "<< value <<endl;
    //TODO 
    //cout << "put --------------------"<<endl;
    if(bk) backup.insert_KV(key,value); //备份key
    else self_data.insert_KV(key,value);
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
    if( self_data.check_K(key)) self_data.update_KV(key,value);
    if(backup.check_K(key)) backup.update_KV(key,value);
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
    if( self_data.check_K(key)) self_data.delete_K(key);
    if( backup.check_K(key)) backup.delete_K(key);
    json res;
    res["type"] = SLAVE_SEVER_DELETE_ACK; //发送连接数据
    res["code"] = 0;
    string sendBuf = res.dump();
    conn->send(sendBuf);
}

//接受来自cosever的备份消息 - 向下一个节点做backup的备份
void slaveService::Slave_BACKUP(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    bool only_one = js["only_one"].get<bool>();
    if(only_one) //把backup和自己的合并
    {
        vector<pair<string,string>> send_bk = backup.get_all_data();
        for(auto x: send_bk)
        {
            self_data.insert_KV(x.first,x.second);
        }
        return;
    }
    string ip = js["ip"].get<string>(); //下一个节点
    int port = js["port"].get<int>();
    string pre_ip = js["pre_ip"].get<string>();//上一个节点
    int pre_port = js["pre_port"].get<int>();

    string ip_self = js["ip_self"].get<string>();//本节点
    int port_self = js["port_self"].get<int>();
    
    shared_ptr<caclient> cli = make_shared<caclient>(ip,port);

    vector<pair<string,string>> send_bk = backup.get_all_data();
    json jsp;
    jsp["type"] = BACKUP_RECV;
    jsp["back_up"] = send_bk;
    string sendBuf = js.dump();
    cli->ca_send_message(sendBuf);
    cli->ca_receive_message();
    backup.clear();

    //向上上一个节点提出备份 TODO - 备份的是上一个节点自己的数据
    cli->ca_closing();

    shared_ptr<caclient> pre_cli = make_shared<caclient>(pre_ip,pre_port);
    json jsn;
    jsn["type"] = BACKUP_PRE;
    jsn["ip"] = ip_self;
    jsn["port"] = port_self;
    string se = jsn.dump();
    pre_cli->ca_send_message(se);
    pre_cli->ca_closing();
    
}

//接受来自slave_server的备份消息，接受来自上一个节点的备份,备份到backup中
void slaveService::Slave_BACKUP_RECV(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    vector<pair<string,string>> pre_backup = js["back_up"].get<vector<pair<string,string>>>();
    for(pair<string,string> bk : pre_backup)
    {
        backup.insert_KV(bk.first,bk.second);
    }
    json jsp;
    jsp["type"] = BACKUP_RECV_ACK;
    string sendBuf = jsp.dump();
    conn->send(sendBuf);
}

//我当作pre节点，向下一个节点备份我的self_data
void slaveService::Slave_BACKUP_PRE(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string ip = js["ip"].get<string>();
    int port = js["port"].get<int>();
    shared_ptr<caclient> cli = make_shared<caclient>(ip,port);

    vector<pair<string,string>> send_bk = self_data.get_all_data();
    json jsp;
    jsp["type"] = BACKUP_RECV;
    jsp["back_up"] = send_bk;
    string sendBuf = js.dump();
    cli->ca_send_message(sendBuf);
    cli->ca_receive_message();
    //向上上一个节点提出备份 TODO - 备份的是上一个节点自己的数据
    cli->ca_closing();
}


slaveService::slaveService()
{
    // 用户基本业务管理相关事件处理回调注册
    _msgHandlerMap.insert({CLIENT_GET, std::bind(&slaveService::Slave_GET, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_PUT, std::bind(&slaveService::Slave_PUT, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_UPDATE, std::bind(&slaveService::Slave_UPDATE, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_DELETE, std::bind(&slaveService::Slave_DELETE, this, _1, _2, _3)});
    _msgHandlerMap.insert({BACKUP_CO, std::bind(&slaveService::Slave_BACKUP, this, _1, _2, _3)});
    _msgHandlerMap.insert({BACKUP_RECV, std::bind(&slaveService::Slave_BACKUP_RECV, this, _1, _2, _3)});
    _msgHandlerMap.insert({BACKUP_PRE, std::bind(&slaveService::Slave_BACKUP_PRE, this, _1, _2, _3)});

}