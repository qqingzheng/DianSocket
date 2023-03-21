/*
 * AlSocket.h
 *
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "CommonDefs.h"
#include <unistd.h>
#include <tuple>
#include <iostream>
#include <map>
#include <netinet/in.h>

#define INVALID_SOCKET -1
#define SOCK_MAX_RETRY 10
#define MAX_CONNECT_CLIENT 5

static const std::map<const char*, uint16_t> mapSockType = {
        {"SOCK_STREAM", SOCK_STREAM}, {"SOCK_SEQPACKET", SOCK_SEQPACKET}, {"SOCK_DGRAM", SOCK_DGRAM}};
static const std::map<const char*, uint16_t> mapProtoType = {
        {"IPPROTO_TCP", IPPROTO_TCP}, {"IPPROTO_SCTP", IPPROTO_SCTP}, {"IPPROTO_UDP", IPPROTO_UDP}};

typedef struct
{
    uint16_t protoType{IPPROTO_SCTP};  // IPPROTO_TCP/UDP/SCTP
    char     HostName[64];
    uint32_t IpAddr;
    uint16_t Port;
    uint16_t socketType{SOCK_STREAM};
    int32_t  timetolive{0};
} SocketParamStruct;
typedef struct
{
    uint16_t                protoType;
    int16_t                 socketType;
    // 用于Socket服务端
    struct sockaddr_storage addr;
    char                    name[64];
    int32_t                 addrLen;
    int32_t                 port;
    // 用于Socket客户端。_r后缀代表remote的意思，代表连接的服务器信息。
    struct sockaddr_storage addr_r;
    char                    name_r[64];
    int32_t                 addrLen_r;
    int32_t                 port_r;
    int32_t                 timetolive;
} SocketCommonCfgStruct;
typedef struct
{
    int32_t        sockFd;
    SocketCommonCfgStruct sCommonCfg;
} SocketCfgStruct;

class Socket
{
public:
    Socket();
    virtual ~Socket();
    int get_socket_fd()
    {
        return sSocketConfig.sockFd;
    };
    int get_socket_type()
    {
        return sSocketConfig.sCommonCfg.socketType;
    };
    const char* getHostName(bool isLocal = true) const;
    int         getPort(bool isLocal = true);

    virtual void init_local_host(const SocketParamStruct* sSockParam) final;
    virtual void init_remote_host(const SocketParamStruct* sSockParam) final;
    int32_t      create_socket();
    int32_t      sbind();
    int32_t      set_block(int32_t IsBlock);
    int32_t      sclose();
    void         set_send_recv_buf(int sendbuf, int recvbuf);

    virtual bool saccept(int32_t listenSockFd)
    {
        return false;
    };
    virtual int32_t sconnect()
    {
        return 0;
    };
    virtual int slisten()
    {
        return 0;
    };
    virtual ssize_t  ssend(void* msg, size_t msg_len)    = 0;
    virtual ssize_t  srecv(void* recvbuf, size_t buflen) = 0;

protected:
    SocketCfgStruct sSocketConfig;

protected:
    virtual void set_socket_fd(int32_t sockFd) final
    {
        sSocketConfig.sockFd = sockFd;
    };
};

#endif
