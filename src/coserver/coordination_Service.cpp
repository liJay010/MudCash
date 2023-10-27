#include "coordination_Service.h"
#include "global.h"
#include "caclient.h"
#include <memory>
#include <iostream>
#include "Logger.h"
using namespace std;
using namespace placeholders;

string faild_ACK()
{
    //请求失败
    json jsr;
    jsr["type"] = SLAVE_SEVER_PUT_ACK; //发送连接数据
    jsr["code"] = MESSAGE_FALL; //发送连接数据
    return  jsr.dump();
} 
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
    if(_fdMap.empty())
    {
        conn->send(faild_ACK());
        return;
    }

    string key = js["key"].get<string>();

    hash_Node* pre = nullptr;
    hash_Node* next = nullptr;//使用这个
    hash_Node* next_next= nullptr;
    Suc(root, pre,next, CRC32(key));
    if(next == nullptr) next = minValue(root);
    if(next == nullptr)
    {
        cout << "can not find next -> "<<endl;
        exit(1);
    }
    Suc(root, pre,next_next, next->key);
    if(next_next == nullptr) next_next = minValue(root);
    if(_fdMap.find(next->ip_plus_port) == _fdMap.end())
    {
        cout << "can not find node -> " +next->ip_plus_port<<endl;
        exit(1);
    }
    cout << "key: " << key << "ip_plus_port Server " << next->ip_plus_port <<endl;
    //向next发送
    js["back_up"] = false;
    shared_ptr<caclient> cli= _fdMap[next->ip_plus_port];
    
    string sendBuf = js.dump();
    cli->ca_send_message(sendBuf);
    RECV res = cli->ca_receive_message();
    RECV res_cli;
    if(next_next != next && next_next != nullptr)
    {
        js["back_up"] = true;
        sendBuf = js.dump();
        shared_ptr<caclient> cli_next= _fdMap[next_next->ip_plus_port];
        cli_next->ca_send_message(sendBuf);
        res_cli = cli_next->ca_receive_message();

        if(res.cnt > 0 && res_cli.cnt > 0) conn->send(res.res);
        else conn->send(faild_ACK());
    }
    else{
        if(res.cnt > 0) conn->send(res.res);
        else conn->send(faild_ACK());
    }
}
void coordination_Service::client_get(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    if(_fdMap.empty())
    {
        conn->send(faild_ACK());
        return;
    }
    string key = js["key"].get<string>();
    hash_Node* pre = nullptr;
    hash_Node* next = nullptr;//使用这个
    hash_Node* next_next= nullptr;
    Suc(root, pre,next, CRC32(key));
    if(next == nullptr) next = minValue(root);

    if(next == nullptr)
    {
        cout <<_fdMap.size() << " - size of fd can not find next -> null"<<endl;
        exit(1);
    }
    Suc(root, pre,next_next, next->key);
    if(next_next == nullptr) next_next = minValue(root);
    if(_fdMap.find(next->ip_plus_port) == _fdMap.end())
    {
        cout << "can not find node -> " +next->ip_plus_port<<endl;
        exit(1);
    }

    //向next发送
    shared_ptr<caclient> cli= _fdMap[next->ip_plus_port];
    string sendBuf = js.dump();
    cli->ca_send_message(sendBuf);
    RECV res = cli->ca_receive_message();
    

    if(res.cnt > 0) 
    {
        conn->send(res.res);
        return;
    }
    else//掉线了,需要备份，发送消息给下一个节点
    {
        root = deletehash_Node(root, next->key);
        _fdMap.erase(cli->ip_port_self);
        //需要告知其下下一个节点的ip以及端口，讲数据发送，然后删除backup中数据
        if(next_next)
        {
            
            //向下下一个节点备份数据
            hash_Node* sub_next= nullptr;
            Suc(root, pre,sub_next, CRC32(next_next->ip_plus_port));

            if(sub_next == nullptr) sub_next = minValue(root);
            if(pre == nullptr) pre = maxValue(root);
            if(sub_next == next_next)//说明就只有一个节点了
            {
                cout << "just one node" <<endl;
                shared_ptr<caclient> cli_next= _fdMap[next_next->ip_plus_port];
                json jsp;
                jsp["type"] = BACKUP_CO; //发送连接数据
                jsp["only_one"] = true; 
                string sendBufp = jsp.dump();
                cli_next->ca_send_message(sendBufp);
            }
            else if(pre && sub_next && sub_next != next_next && pre != next_next)
            {
                json jsp;
                shared_ptr<caclient> cli_next= _fdMap[next_next->ip_plus_port];
                if(_fdMap.find(pre->ip_plus_port) == _fdMap.end())
                {
                    cout << "sub_next -> cli null" <<endl;
                    exit(0);
                }
                shared_ptr<caclient> pre_cli_next= _fdMap[pre->ip_plus_port];
                if(_fdMap.find(sub_next->ip_plus_port) == _fdMap.end())
                {
                    cout << "sub_next -> cli null" <<endl;
                    exit(0);
                }
                shared_ptr<caclient> sub_cli_next= _fdMap[sub_next->ip_plus_port];
                jsp["type"] = BACKUP_CO; //发送连接数据
                jsp["only_one"] = false; 
                jsp["ip_self"] = cli_next->ip_self;
                jsp["port_self"] = cli_next->port_self;
                jsp["ip"] = sub_cli_next->ip_self;
                jsp["port"] = sub_cli_next->port_self;
                jsp["pre_ip"] = pre_cli_next->ip_self;
                jsp["pre_port"] = pre_cli_next->port_self;

                cout << cli->ip_port_self << "掉线 -> 备份到下一个节点 ："<< next_next->ip_plus_port << endl;
                cout << "pre ip : "<< pre_cli_next->ip_self << " port :" << pre_cli_next->port_self << endl;
                cout << "cur ip : "<< cli_next->ip_self << " port :" << cli_next->port_self << endl;
                cout << "next ip : "<< sub_cli_next->ip_self << " port :" << sub_cli_next->port_self << endl;
                string sendBufp = jsp.dump();
                cli_next->ca_send_message(sendBufp);
            }
            
        }
    }

    if(next_next == next || next_next == nullptr)
    {
        conn->send(faild_ACK());
        return;
    }

    shared_ptr<caclient> cli_next= _fdMap[next_next->ip_plus_port];
    RECV res_cli;
    cli_next->ca_send_message(sendBuf);

    RECV res_next = cli_next->ca_receive_message();
    if(res_next.cnt > 0) 
    {
        conn->send(res_next.res);
        return;
    }
    else 
    {
        conn->send(faild_ACK());
    }

}
void coordination_Service::client_delete(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    client_get(conn, js, time);
}
void coordination_Service::client_update(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    client_get(conn, js, time);
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
    cli->ip_port_self = name;
    cli->ip_self = ip;
    cli->port_self = stoi(port);
    root = insert(root, CRC32(name), name);
    inorder(root); //打印哈希环

    /*
    插入B
    此处需要查询节点-若之前无节点，直接添加，
    若之前有一个节点A：
        遍历A，将所有节点选择出属于B的节点。将B节点作为backup。选择出属于A的节点。
        将A，B节点分别发送给B，A作为B的backup

    若有两个节点及以上A -> pre C->next：
        遍历C节点，选出属于B的节点，将B节点作为C的新的backup。将原来的backup作为B的Backup
        */

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
coordination_Service::coordination_Service():root(nullptr)
{

    // 用户基本业务管理相关事件处理回调注册
    _msgHandlerMap.insert({CLIENT_CONNECT, std::bind(&coordination_Service::clientACK, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_PUT, std::bind(&coordination_Service::client_put, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_GET, std::bind(&coordination_Service::client_get, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_DELETE, std::bind(&coordination_Service::client_delete, this, _1, _2, _3)});
    _msgHandlerMap.insert({CLIENT_UPDATE, std::bind(&coordination_Service::client_update, this, _1, _2, _3)});

    _msgHandlerMap.insert({SLAVE_SEVER_CONNECT, std::bind(&coordination_Service::slave_ACK, this, _1, _2, _3)});
    
}

