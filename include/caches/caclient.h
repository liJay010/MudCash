#pragma once
#include <string>

struct RECV
{
    std::string res;
    int cnt;
};

class caclient
{
public:
    //连接服务器的 ip和端口 -- 
    caclient(std::string ip,int port);
    int ca_connecting(); //返回connnect的fd
    //开始启动服务
    void ca_start();
    void ca_closing();
    void ca_send_message(std::string  msg);
    RECV ca_receive_message();


    int fd_; 
    std::string ip;//服务器端ip
    int port;//服务器端端口
    std::string ip_port_self;//服务器端ip
    std::string ip_self;
    int port_self;
};

