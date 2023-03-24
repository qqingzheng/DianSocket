#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "SctpSocket.h"
int32_t SctpSocket::create_socket(){
    Socket::create_socket();
    return sSocketConfig.sockFd;
}

int SctpSocket::slisten()
{
    int32_t error = 0;
    if (sSocketConfig.sockFd == 0 || sSocketConfig.sockFd == INVALID_SOCKET)
    {
        LOG_ERROR("sockFd is invalid! \n");
        exit(1);
    }
    error = listen(sSocketConfig.sockFd, MAX_CONNECTIONS);
    LOG_INFO("Listening in sockFd=%d\n", sSocketConfig.sockFd);
    if (error != 0)
    {
        LOG_ERROR("listen error: %s\n", strerror(errno));
        exit(1);
    }    
    return 0;
}

int32_t SctpSocket::sconnect()
{
    int32_t             error = 0;
    char                ipAddress[INET_ADDRSTRLEN];
    struct sockaddr_in* temp_addr = (struct sockaddr_in*)&sSocketConfig.sCommonCfg.addr_r;
    inet_ntop(AF_INET, &(temp_addr->sin_addr), ipAddress, INET_ADDRSTRLEN);

    LOG_INFO("SctpSocket::sctp_connect: sockFd=%d -> %s\n", sSocketConfig.sockFd, ipAddress);
    int attempts = 0;

    do
    {
        if (attempts > 0)
        {
            sleep(1); /* sleep for retry... */
        }
        error = connect(sSocketConfig.sockFd, (struct sockaddr*)temp_addr, sSocketConfig.sCommonCfg.addrLen_r);
        LOG_INFO("connecting to %s:%d, rc=%d\n.", sSocketConfig.sCommonCfg.name_r, sSocketConfig.sCommonCfg.port_r,
                    errno);
        if (error < 0)
        {
            LOG_ERROR("connect: %s.\n", strerror(errno));
        }
        attempts++;
    } while (error < 0 && errno != EISCONN && attempts < SCTP_MAX_RETRY);

    if (attempts >= SCTP_MAX_RETRY)
    {
        LOG_ERROR("connect: max try reached, stop connecting\n");
        return -1;
    }

    return 0;
}

bool SctpSocket::saccept(int32_t listenSockFd)
{
    if (listenSockFd == 0 || listenSockFd == INVALID_SOCKET)
    {
        LOG_ERROR("sockfd is invalid! \n");
        return false;
    }
    int32_t            clientSocketFd;
    struct sockaddr_in clientAddress;
    socklen_t          addressLen = sizeof(struct sockaddr);
    clientSocketFd = accept(listenSockFd, (struct sockaddr*)&clientAddress, &addressLen);
    if (clientSocketFd == -1)
    {
        LOG_ERROR("sockfd accept fail! listenSockFd=%d\n", listenSockFd);
        return false;
    }
    set_socket_fd(clientSocketFd);
    struct   addrinfo*     res = NULL;
    char*    pClientName       = sSocketConfig.sCommonCfg.name_r;
    int32_t& rClientPort       = sSocketConfig.sCommonCfg.port_r;

    // https://blog.csdn.net/andrewgithub/article/details/81812458 了解getpeername和getsockname的含义
    if (!getpeername(clientSocketFd, (struct sockaddr*)&clientAddress, &addressLen))
    {
        uint32_t sinAddrLen = strlen(inet_ntoa(clientAddress.sin_addr));
        if (inet_ntoa(clientAddress.sin_addr) != NULL && sinAddrLen < sizeof(sSocketConfig.sCommonCfg.name_r))
        {
            memcpy(pClientName, inet_ntoa(clientAddress.sin_addr), sinAddrLen);
        }
        rClientPort = ntohs(clientAddress.sin_port);
    }
    int32_t error = getaddrinfo(pClientName, 0, NULL, &res);
    if (error || res == NULL)
    {
        LOG_ERROR("%s.\n", gai_strerror(error));
        exit(1);
    }
    switch (res->ai_family)
    {
    case AF_INET:{
        struct sockaddr_in* pTempAddr = (struct sockaddr_in*)&sSocketConfig.sCommonCfg.addr_r;
        memcpy(pTempAddr, res->ai_addr, res->ai_addrlen);
        pTempAddr->sin_family = res->ai_family;
        pTempAddr->sin_port   = htons(rClientPort);
        // temp_addr->sin_addr = res;
        sSocketConfig.sCommonCfg.addrLen_r = res->ai_addrlen;
        break;
    }
    case AF_INET6:{
        struct sockaddr_in6*  pTempAddr6 = (struct sockaddr_in6*)&sSocketConfig.sCommonCfg.addr_r;
        memcpy(pTempAddr6, res->ai_addr, res->ai_addrlen);
        pTempAddr6->sin6_family          = res->ai_family;
        pTempAddr6->sin6_port            = htons(rClientPort);
        sSocketConfig.sCommonCfg.addrLen_r = res->ai_addrlen;
        break;
    }
    default:
        exit(1);
    }
    freeaddrinfo(res);
    return true;
}

ssize_t SctpSocket::ssend(void* message, size_t msg_len)
{
    struct msghdr           sndMsgHdr;
    char                    outCtrlMsg[CMSG_SPACE(sizeof(struct sctp_sndrcvinfo))];
    struct sctp_sndrcvinfo* sctp_info;

    struct iovec vIoData;
    vIoData.iov_base = message;
    vIoData.iov_len  = msg_len;

    sndMsgHdr.msg_name       = &sSocketConfig.sCommonCfg.addr_r;
    sndMsgHdr.msg_namelen    = sizeof(struct sockaddr_storage);
    sndMsgHdr.msg_iov        = &vIoData;
    sndMsgHdr.msg_iovlen     = 1;
    sndMsgHdr.msg_control    = outCtrlMsg;
    sndMsgHdr.msg_controllen = sizeof(outCtrlMsg);
    sndMsgHdr.msg_flags      = 0;

    struct cmsghdr* ctrlMsg;
    ctrlMsg             = CMSG_FIRSTHDR(&sndMsgHdr);
    ctrlMsg->cmsg_level = IPPROTO_SCTP;
    ctrlMsg->cmsg_type  = SCTP_SNDRCV;
    ctrlMsg->cmsg_len   = CMSG_LEN(sizeof(struct sctp_sndrcvinfo));

    sndMsgHdr.msg_controllen = ctrlMsg->cmsg_len;
    sctp_info                = (struct sctp_sndrcvinfo*)CMSG_DATA(ctrlMsg);
    memset(sctp_info, 0, sizeof(struct sctp_sndrcvinfo));
    sctp_info->sinfo_ppid       = rand();
    sctp_info->sinfo_stream     = 0;
    sctp_info->sinfo_flags      = 0;
    sctp_info->sinfo_flags      = SCTP_UNORDERED;
    sctp_info->sinfo_timetolive = 0;

    // 由于当发送信息时连接关闭，操作系统会向进程发送SIGPIPE信号，而如果线程或者进程未处理这个信号，就会被操作系统杀掉。
    // 这里加入MSG_NOSIGNAL flag可以防止操作系统发送SIGPIPE信号。
    // MSG_WAITALL flag等待所有消息发送完才返回
    ssize_t rtv = sendmsg(sSocketConfig.sockFd, &sndMsgHdr, MSG_WAITALL | MSG_NOSIGNAL);
    if (rtv != (ssize_t)msg_len)
    {
        int sendbuf = 0;
        int len     = sizeof(sendbuf);
        getsockopt(sSocketConfig.sockFd, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t*)&len);
        LOG_ERROR("\n\t\t*** sendmsg: %s, rtv=%zd, msg_len=%zu, sendbuf size=%d ***\n\n", strerror(errno), rtv,
                     msg_len, sendbuf);
        return -1;
    }

    return rtv;
}

ssize_t SctpSocket::srecv(void* recvbuf, size_t buflen)
{
    int32_t         recvSocketFd    = sSocketConfig.sockFd;
    int32_t         lenRecv   = 0;
    int             msgFlags = 0;
    sctp_sndrcvinfo sSndRcvinfo;
    lenRecv = sctp_recvmsg(recvSocketFd, recvbuf, buflen, (struct sockaddr*)NULL, 0, &sSndRcvinfo, &msgFlags);
    if (lenRecv < 0)
    {
        if (errno == EAGAIN)
        {
            // nothing recv by the non-blocking recvmsg call. do nothing
            return 0;
        }
        LOG_ERROR(" recvmsg: %s \n\n", strerror(errno));
        return -1;
    }
    else if (lenRecv == 0)
    {
        LOG_ERROR("\n\t\tConnnection closed.\n");
        return 0;
    }
    LOG_INFO("sSndRcvinfo, assoc_id:%d, flag: %d\n", sSndRcvinfo.sinfo_assoc_id, sSndRcvinfo.sinfo_flags);
    return lenRecv;
}

