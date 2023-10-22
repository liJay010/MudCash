#include "json.hpp"
using json = nlohmann::json;
#include "global.h"
#include <iostream>
#include <vector>
#include <map>
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<sys/types.h> 
#include<errno.h>
#include<unistd.h> 
#include<cstring>
#include<pthread.h>
#include "caclient.h"
using namespace std;





caclient::caclient(std::string ip,int port):ip(ip),port(port)
{
    // 创建一个TCP套接字
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
    {
        perror("socket error");
    }
}
int caclient::ca_connecting()
{
    // 连接到服务器端的套接字
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddr.sin_port = htons(port);

    if (connect(fd_, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        perror("connect error");
        return -1;
    }
    return fd_;
}

//开始启动服务
void caclient::ca_start()
{
    ca_connecting();
    //receive_message(fd_);// ack + connected -- 接受连接成功的信息
    json js;
    js["type"] = CLIENT_CONNECT; //发送连接数据
    string sendBuf = js.dump();
    ca_send_message(sendBuf);

    cout << ca_receive_message() <<endl;;// ack + ready_to_serve -- 接受连接成功的信息

    
    while (true)
    {
        string command;
        cout << "command > ";
        cin >> command;

        if(command=="exit")
        {
     	    cout<<"Bye~"<<endl;
            close(fd_);
     	    return;
        }
        char *command_char=(char*)command.c_str();
        string type=strtok(command_char,":");

        if(type=="get"||type=="delete") // 后面只有一个参数
        {
            char *get_char=strtok(NULL,":");
     	    string key=get_char;
            js["type"] = type=="get" ? CLIENT_GET : CLIENT_DELETE; //发送连接数据
            js["key"] = key;

            sendBuf = js.dump();
            ca_send_message(sendBuf);
            cout << ca_receive_message() <<endl;
     	    //request_for_get_delete(type,key,fd_);
        } 

        if(type=="put"||type=="update") //两个参数
        {
            char *get_char=strtok(NULL,":");
     	    string key=get_char;
        	char *get_val=strtok(NULL,":");
     	    string value=get_val;
            js["type"] = type=="put" ? CLIENT_PUT : CLIENT_UPDATE; //发送连接数据
            js["key"] = key;
            js["value"] = value;

            sendBuf = js.dump();
            ca_send_message(sendBuf);
            cout << ca_receive_message() <<endl;
            //request_for_update_put(type,key,value,fd_);
        }
        cout<<endl;
    }
    
}
void caclient::ca_closing()
{
    close(fd_);
}

//发送消息
void caclient::ca_send_message(string  msg)
{ 
  	char *char_pointer=(char*)msg.c_str();
  	char char_array[BUFF_SIZE];
  	strcpy(char_array,char_pointer);
  	send(fd_,char_pointer,sizeof(char_array),0);
}
//接受消息
string caclient::ca_receive_message()
{   
	char Received_msg[BUFF_SIZE]={0};
    recv(fd_,Received_msg,sizeof(Received_msg),0);
    return string(Received_msg);
}
