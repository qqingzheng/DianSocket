#include "Socket.h"
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
Socket::Socket()
{
    sSocketConfig.sockFd                = -1;
    sSocketConfig.sCommonCfg.socketType = SOCK_STREAM;
    sSocketConfig.sCommonCfg.timetolive = 0;
    sSocketConfig.sCommonCfg.addrLen    = 0;
    sSocketConfig.sCommonCfg.addr       = {0};
    sSocketConfig.sCommonCfg.port       = 0;
    memset(sSocketConfig.sCommonCfg.name, 0, sizeof(sSocketConfig.sCommonCfg.name));
}

int32_t Socket::create_socket(){

      // Create Socket
    int32_t socketFd;
    socketFd = socket(sSocketConfig.sCommonCfg.addr.ss_family, 
    sSocketConfig.sCommonCfg.socketType, sSocketConfig.sCommonCfg.protoType);
    LOG_INFO("ProtoType: %d  SocketType: %d", sSocketConfig.sCommonCfg.socketType, sSocketConfig.sCommonCfg.protoType);

    if(socketFd < 0){
        LOG_ERROR("Failed to create socket!");
        exit(1);
    }

    LOG_INFO("Socket created, fd= %d", socketFd);
    return socketFd;
}

void Socket::init_local_host(const SocketParamStruct* sSockParam){
    char                                hostStr[NI_MAXHOST];
    char                                servStr[NI_MAXSERV];
    struct addrinfo*                    res;
    int32_t                             error;
    SocketCommonCfgStruct& sCommonCfg = sSocketConfig.sCommonCfg;

    strncpy(sCommonCfg.name, sSockParam->HostName, sizeof(sCommonCfg.name));  // copy hostname
    sCommonCfg.protoType  = sSockParam->protoType;
    sCommonCfg.socketType = sSockParam->socketType;

    error                               = getaddrinfo(sSockParam->HostName, 0, NULL, &res);
    if(error){
        LOG_ERROR("%s.\n", gai_strerror(error));
    }
    
    switch(res->ai_family){
        case AF_INET :{   // IPv4
            struct sockaddr_in* tempAddr = (struct sockaddr_in*) &sCommonCfg.addr;
            tempAddr->sin_family = res->ai_family;
            memcpy(&tempAddr->sin_addr, res->ai_addr, res->ai_addrlen);
            inet_pton(AF_INET, sCommonCfg.name, &tempAddr->sin_addr);
            tempAddr->sin_port = htons(sSockParam->Port);
            sCommonCfg.addrLen = res->ai_addrlen;
            break;
        }
        case AF_INET6:{   //IPv6
            struct sockaddr_in6* tempAddr6 = (struct sockaddr_in6*) &sCommonCfg.addr;
            tempAddr6->sin6_family = res->ai_family;
            memcpy(&tempAddr6->sin6_addr, res->ai_addr, res->ai_addrlen);
            inet_pton(AF_INET6, sCommonCfg.name, &tempAddr6->sin6_addr);
            tempAddr6->sin6_port = htons(sSockParam->Port);
            sCommonCfg.addrLen = res->ai_addrlen;
            break;
        }
        default:{
            LOG_ERROR("Fail to init local host!");
            exit(1);
        }
    }
    // https://www.jianshu.com/p/eb4223f06006 关于getnameinfo函数的资料。
    error = getnameinfo((struct sockaddr*)&sCommonCfg.addr, sCommonCfg.addrLen, hostStr, NI_MAXHOST, servStr,
                        NI_MAXSERV, NI_NUMERICHOST);
    if (error)
    {
        LOG_ERROR("%s\n", gai_strerror(error));
        exit(1);
    }
    LOG_INFO("local:@=%s, port=%s, family=%d\n", hostStr, servStr, res->ai_family);
    freeaddrinfo(res);
}

void Socket::init_remote_host(const SocketParamStruct* sSockParam){
    char                                hostStr[NI_MAXHOST];
    char                                servStr[NI_MAXSERV];
    struct addrinfo*                    res;
    int32_t                             error;
    SocketCommonCfgStruct& sCommonCfg = sSocketConfig.sCommonCfg;

    strncpy(sCommonCfg.name_r, sSockParam->HostName, sizeof(sCommonCfg.name_r));  // copy hostname
    sCommonCfg.protoType  = sSockParam->protoType;
    sCommonCfg.socketType = sSockParam->socketType;

    error                               = getaddrinfo(sSockParam->HostName, 0, NULL, &res);
    if(error){
        LOG_ERROR("%s.\n", gai_strerror(error));
    }
    
    switch(res->ai_family){
        case AF_INET :{   // IPv4
            struct sockaddr_in* tempAddr = (struct sockaddr_in*) &sCommonCfg.addr_r;
            tempAddr->sin_family = res->ai_family;
            memcpy(&tempAddr->sin_addr, res->ai_addr, res->ai_addrlen);
            inet_pton(AF_INET, sCommonCfg.name, &tempAddr->sin_addr);
            tempAddr->sin_port = htons(sSockParam->Port);
            sCommonCfg.addrLen_r = res->ai_addrlen;
            break;
        }
        case AF_INET6:{   //IPv6
            struct sockaddr_in6* tempAddr6 = (struct sockaddr_in6*) &sCommonCfg.addr_r;
            tempAddr6->sin6_family = res->ai_family;
            memcpy(&tempAddr6->sin6_addr, res->ai_addr, res->ai_addrlen);
            inet_pton(AF_INET6, sCommonCfg.name, &tempAddr6->sin6_addr);
            tempAddr6->sin6_port = htons(sSockParam->Port);
            sCommonCfg.addrLen_r = res->ai_addrlen;
            break;
        }
        default:{
            LOG_ERROR("Fail to init local host!");
            exit(1);
        }
    }
    // https://www.jianshu.com/p/eb4223f06006 关于getnameinfo函数的资料。
    error = getnameinfo((struct sockaddr*)&sCommonCfg.addr_r, sCommonCfg.addrLen_r, hostStr, NI_MAXHOST, servStr,
                        NI_MAXSERV, NI_NUMERICHOST);
    if (error)
    {
        LOG_ERROR("%s\n", gai_strerror(error));
        exit(1);
    }
    LOG_INFO("remote:@=%s, port=%s, family=%d\n", hostStr, servStr, res->ai_family);
    freeaddrinfo(res);
}


int32_t Socket::sbind(){
    int32_t error = 0;
    int32_t i     = 0;
    char       host_str[NI_MAXHOST];
    char       serv_str[NI_MAXSERV];

    struct sockaddr_storage* saddr    = &sSocketConfig.sCommonCfg.addr;
    int32_t                  addr_len = sSocketConfig.sCommonCfg.addrLen;

    do{
        if (i > 0)
        {
            // 等待重试
            LOG_INFO("Waiting for retry...%d", i);
            sleep(1); 
        }
        error = getnameinfo((struct sockaddr*)saddr, addr_len, host_str, NI_MAXHOST, serv_str, NI_MAXSERV,
                            NI_NUMERICHOST);
        if (error)
        {
            LOG_ERROR("%s\n", gai_strerror(error));
        }
        LOG_INFO("\tbind(sockFd=%d, [a:%s,p:%s])  --  attempt %d/%d\n", sSocketConfig.sockFd, host_str, serv_str,
                    i + 1, SOCK_MAX_RETRY);
        error = bind(sSocketConfig.sockFd, (struct sockaddr*)saddr, addr_len);

        if (error != 0)
        {
            if (errno != EADDRINUSE)
            {
                LOG_ERROR("\nbind: can not bind to %s:%s: %s \n", host_str, serv_str, strerror(errno));
                exit(1);
            }
        }
        i++;
        if (i >= SOCK_MAX_RETRY)
        {
            LOG_ERROR("failed in bind()\n\n");
            return (-1 * EADDRINUSE);
        }
    } while (error < 0 && i < SOCK_MAX_RETRY);
    return 0;
}

int32_t Socket::sclose()
{
    int32_t error = 0;
    LOG_INFO("\tclose(sockFd=%d)\n", sSocketConfig.sockFd);
    error = close(sSocketConfig.sockFd);
    if (error != 0)
    {
        LOG_ERROR("\n close: %s \n\n", strerror(errno));
        exit(1);
    }
    set_socket_fd(-1);
    return 0;
}

void Socket::set_send_recv_buf(int sendbuf, int recvbuf)
{
    if (sendbuf != -1)
    {
        int len = sizeof(sendbuf);
        setsockopt(sSocketConfig.sockFd, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof(sendbuf));
        getsockopt(sSocketConfig.sockFd, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t*)&len);
        LOG_INFO("sock %d send buf size set to:%d.\n", sSocketConfig.sockFd, sendbuf);
    }

    if (recvbuf != -1)
    {
        int len = sizeof(recvbuf);
        setsockopt(sSocketConfig.sockFd, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof(recvbuf));
        getsockopt(sSocketConfig.sockFd, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t*)&len);
        LOG_INFO("sock %d  recv buf size set to:%d.\n", sSocketConfig.sockFd, recvbuf);
    }
}

