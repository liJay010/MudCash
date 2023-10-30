#include "slaveService.h"
#include "global.h"
#include "caclient.h"
#include "avl.hpp"
using namespace std;
using namespace placeholders;

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
    vector<pair<string,string>> send_bk = backup.get_all_data();
    for(auto x: send_bk)
    {
        self_data.insert_KV(x.first,x.second);
    }
    if(only_one) //把backup和自己的合并
    {
        backup.clear();
        cout << "self data" <<endl;
        self_data.show();
        return;
    }
    string ip = js["ip"].get<string>(); //下一个节点
    int port = js["port"].get<int>();
    string pre_ip = js["pre_ip"].get<string>();//上一个节点
    int pre_port = js["pre_port"].get<int>();

    string ip_self = js["ip_self"].get<string>();//本节点
    int port_self = js["port_self"].get<int>();
    /*cout << "节点备份"<< endl;
    cout << "pre ip : "<< pre_ip << " port :" << pre_port << endl;
    cout << "cur ip : "<< ip_self << " port :" << port_self << endl;
    cout << "next ip : "<< ip << " port :" << port << endl;*/
    
    shared_ptr<caclient> cli = make_shared<caclient>(ip,port);
    cli->ca_connecting();
    //cout << "next conneted" << endl;

    //vector<pair<string,string>> send_bk = backup.get_all_data();
    //cout << "data sucess" << endl;
    json jsp;
    jsp["type"] = BACKUP_RECV;
    jsp["back_up"] = send_bk;
    string sendBuf = jsp.dump();
    //cout << "dump sucess" << endl;
    cli->ca_send_message(sendBuf);
    //cout << "send sucess" << endl;
    cli->ca_receive_message();
    backup.clear();
    //cout << "next_backup sucess" << endl;
    //向上上一个节点提出备份 TODO - 备份的是上一个节点自己的数据
    cli->ca_closing();

    shared_ptr<caclient> pre_cli = make_shared<caclient>(pre_ip,pre_port);
    pre_cli->ca_connecting();
    json jsn;
    jsn["type"] = BACKUP_PRE;
    jsn["ip"] = ip_self;
    jsn["port"] = port_self;
    string se = jsn.dump();
    pre_cli->ca_send_message(se);
    pre_cli->ca_closing();
    
    cout << "self data" <<endl;
    self_data.show();
    cout << "backup data" <<endl;
    backup.show();
}

//接受来自slave_server的备份消息，接受来自上一个节点的备份,备份到backup中
void slaveService::Slave_BACKUP_RECV(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    vector<pair<string,string>> pre_backup = js["back_up"].get<vector<pair<string,string>>>();

    cout << "接收备份数据:backup" << endl;
    for(pair<string,string> bk : pre_backup)
    {
        cout << bk.first << " : " << bk.second << endl;
        backup.insert_KV(bk.first,bk.second);
    }
    cout << "接收备份数据 done" << endl;
    json jsp;
    jsp["type"] = BACKUP_RECV_ACK;
    string sendBuf = jsp.dump();
    conn->send(sendBuf);
    //备份完成打印现在的数据
    cout << "self data" <<endl;
    self_data.show();
    cout << "backup data" <<endl;
    backup.show();
}

//我当作pre节点，向下一个节点备份我的self_data
void slaveService::Slave_BACKUP_PRE(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string ip = js["ip"].get<string>();
    int port = js["port"].get<int>();
    shared_ptr<caclient> cli = make_shared<caclient>(ip,port);
    cli->ca_connecting();
    //cout << "self_data to" << ip + " "+to_string(port) <<endl;
    vector<pair<string,string>> send_bk = self_data.get_all_data();
    json jsp;
    jsp["type"] = BACKUP_RECV;
    jsp["back_up"] = send_bk;
    string sendBuf = jsp.dump();
    cli->ca_send_message(sendBuf);
    cli->ca_receive_message();
    //向上上一个节点提出备份 TODO - 备份的是上一个节点自己的数据
    cli->ca_closing();
    //cout << "self_data ok"<<endl;
    cout << "self data" <<endl;
    self_data.show();
    cout << "backup data" <<endl;
    backup.show();
}

void slaveService::Slave_NEW_SLAVE2(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string ip = js["ip"].get<string>();
    int port = js["port"].get<int>();
    int key = js["key"].get<int>();
    //vector<pair<string,string>> pre_backup = js["back_up"].get<vector<pair<string,string>>>();

    cout << "节点备份"<< endl;
    cout << "pre ip : "<< ip << " port :" << port << endl;
    cout << "key:"<< key << endl;

    shared_ptr<caclient> cli = make_shared<caclient>(ip,port);
    cli->ca_connecting();
    //cout << "self_data to" << ip + " "+to_string(port) <<endl;
    vector<pair<string,string>> mydata = self_data.get_all_data();
    vector<pair<string,string>> sdata;
    vector<pair<string,string>> bkdata;
    for(pair<string,string> x : mydata)
    {
        if(CRC32(x.first) < key) //属于自己
        {
            sdata.push_back(x);
        }
        else{ //属于下一个节点
            bkdata.push_back(x);
            backup.insert_KV(x.first,x.second);
            self_data.delete_K(x.first);
        }
    }

    json jsp;
    jsp["type"] = NEW_SLAVE2_RECV;
    jsp["back_up"] = bkdata;
    jsp["sdata"] = sdata;
    string sendBuf = jsp.dump();
    cli->ca_send_message(sendBuf);
    cli->ca_closing();
    //cout << "self_data ok"<<endl;
    cout << "self data" <<endl;
    self_data.show();
    cout << "backup data" <<endl;
    backup.show();
}

void slaveService::Slave_NEW_SLAVE2_RECV(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    vector<pair<string,string>> pre_backup = js["back_up"].get<vector<pair<string,string>>>();
    vector<pair<string,string>> pre_self = js["sdata"].get<vector<pair<string,string>>>();

    cout << "接收备份数据:backup" << endl;
    for(pair<string,string> bk : pre_self)
    {
        cout << bk.first << " : " << bk.second << endl;
        backup.insert_KV(bk.first,bk.second);
    }
    for(pair<string,string> bk : pre_backup)
    {
        cout << bk.first << " : " << bk.second << endl;
        self_data.insert_KV(bk.first,bk.second);
    }
    cout << "接收备份数据 done" << endl;

    /*json jsp;
    jsp["type"] = BACKUP_RECV_ACK;
    string sendBuf = jsp.dump();
    conn->send(sendBuf);*/
    //备份完成打印现在的数据
    cout << "self data" <<endl;
    self_data.show();
    cout << "backup data" <<endl;
    backup.show();
}
void slaveService::Slave_NEW_SLAVE_UP2(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string ip = js["ip"].get<string>();
    int port = js["port"].get<int>();
    int key = js["key"].get<int>();
    string next_ip = js["next_ip"].get<string>();
    int next_port = js["next_port"].get<int>();
    //vector<pair<string,string>> pre_backup = js["back_up"].get<vector<pair<string,string>>>();

    cout << "节点备份"<< endl;
    cout << "pre ip : "<< ip << " port :" << port << endl;
    cout << "next ip : "<< next_ip << "next port :" << next_port << endl;
    cout << "key:"<< key << endl;

    shared_ptr<caclient> cli = make_shared<caclient>(ip,port);
    cli->ca_connecting();
    //cout << "self_data to" << ip + " "+to_string(port) <<endl;
    json jsp;
    jsp["type"] = NEW_SLAVE2_RECV;
    
    jsp["sdata"] = backup.get_all_data();// 作为前一个节点的 backup
    cout << "这个作为前一个节点的bk ---------------" <<endl;
    for(auto x : backup.get_all_data()) cout << x.first << " ";
    cout << "这个作为前一个节点的bk -------------" <<endl;
    backup.clear();
    vector<pair<string,string>> mydata = self_data.get_all_data();
    vector<pair<string,string>> bkdata;
    for(pair<string,string> x : mydata)
    {
        if(CRC32(x.first) < key) //不属于自己
        {
            bkdata.push_back(x);
            backup.insert_KV(x.first,x.second);
            self_data.delete_K(x.first);
        }
    }

    cout << "这个作为前一个节点的data ---------------" <<endl;
    for(auto x : bkdata) cout << x.first << " ";
    cout << "这个作为前一个节点的data -------------" <<endl;

    jsp["back_up"] = bkdata; // 作为前一个节点的selfdata
    string sendBuf = jsp.dump();
    cli->ca_send_message(sendBuf);
    cli->ca_closing();

    //向下一个节点删除 bkdata
    json jsr;
    shared_ptr<caclient> cli_next = make_shared<caclient>(next_ip,next_port);
    cli_next->ca_connecting();
    jsr["type"] = DELETE_BK;
    jsr["back_up_delete"] = bkdata;
    string sendBufr = jsr.dump();
    cli_next->ca_send_message(sendBufr);


    //cout << "self_data ok"<<endl;
    cout << "self data" <<endl;
    self_data.show();
    cout << "backup data" <<endl;
    backup.show();
}

void slaveService::Slave_DELETE_BK(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    vector<pair<string,string>> pre_backup_de = js["back_up_delete"].get<vector<pair<string,string>>>();

    cout << "删除备份数据" << endl;
    for(pair<string,string> bk : pre_backup_de)
    {
        backup.delete_K(bk.first);
    }    
    cout << "self data" <<endl;
    self_data.show();
    cout << "backup data" <<endl;
    backup.show();
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
    _msgHandlerMap.insert({NEW_SLAVE2, std::bind(&slaveService::Slave_NEW_SLAVE2, this, _1, _2, _3)});
    _msgHandlerMap.insert({NEW_SLAVE2_RECV, std::bind(&slaveService::Slave_NEW_SLAVE2_RECV, this, _1, _2, _3)});

    _msgHandlerMap.insert({NEW_SLAVE_UP2, std::bind(&slaveService::Slave_NEW_SLAVE_UP2, this, _1, _2, _3)});
    _msgHandlerMap.insert({DELETE_BK, std::bind(&slaveService::Slave_DELETE_BK, this, _1, _2, _3)});


}