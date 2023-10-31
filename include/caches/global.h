#pragma once
#define BUFF_SIZE 1024

/*发送消息类型：type:id,

*/
enum EnMsgType
{
    CLIENT_CONNECT = 1, // client connected 消息
    CLIENT_CONNECTACK, //连接回复
    //客户端请求消息 - coserver
    CLIENT_GET, 
    CLIENT_PUT,
    CLIENT_DELETE,
    CLIENT_UPDATE,

    //slavesever 向 coserver发送的消息，对端回复的消息
    SLAVE_SEVER_CONNECT,
    SLAVE_SEVER_CONNECT_ACK,

    //断连接备份消息
    BACKUP_CO,
    BACKUP_RECV,
    BACKUP_RECV_ACK,
    BACKUP_PRE,

    //新来的slaveserver 连接的消息
    NEW_SLAVE2,
    NEW_SLAVE2_RECV,
    NEW_SLAVE_UP2,
    NEW_SLAVE_UP2_RECV,
    DELETE_BK,

    
};







