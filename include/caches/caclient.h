#pragma once
#include <string>

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
    std::string ca_receive_message();


    int fd_; 
    std::string ip;//服务器端ip
    int port;//服务器端端口
};

