#pragma once
#define BUFF_SIZE 1024

/*发送消息类型：type:id,

*/
enum EnMsgType
{
    CLIENT_CONNECT = 1, // client connected 消息
    CLIENT_CONNECTACK,
    CLIENT_GET,
    CLIENT_PUT,
    CLIENT_DELETE,
    CLIENT_UPDATE,

    CLIENT_GET_ACK,
    CLIENT_PUT_ACK,
    CLIENT_DELETE_ACK,
    CLIENT_UPDATE_ACK,

    SLAVE_SEVER_CONNECT,
    SLAVE_SEVER_CONNECT_ACK,
    SLAVE_SEVER_GET,
    SLAVE_SEVER_PUT,
    SLAVE_SEVER_UPDATE,
    SLAVE_SEVER_DELETE,

    SLAVE_SEVER_GET_ACK,
    SLAVE_SEVER_PUT_ACK,
    SLAVE_SEVER_UPDATE_ACK,
    SLAVE_SEVER_DELETE_ACK,

    //请求失败
    MESSAGE_FALL,
    BACKUP_CO,
    BACKUP_RECV,
    BACKUP_RECV_ACK,
    BACKUP_PRE,

    NEW_SLAVE2,
    NEW_SLAVE2_RECV,
    NEW_SLAVE_UP2,
    NEW_SLAVE_UP2_RECV,
    DELETE_BK,
};







