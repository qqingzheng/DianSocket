
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include "SocketServer.h"
#include "SctpSocket.h"

SocketServer::SocketServer()
{
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        pConnectedSocks[i] = nullptr;
    }
}

SocketServer::~SocketServer()
{
    close();
    if (pListenSock)
        delete pListenSock;
    for (int i = 0; i < MAX_CONNECTIONS; i++)
        if (pConnectedSocks[i])
        {
            delete pConnectedSocks[i];
        }
}
int SocketServer::create()
{
    int32_t fd  = 0;
    int     ret = 0;
    switch (socketParams.protoType)
    {
    case IPPROTO_SCTP:
        LOG_INFO("creating Listening Socket, type is IPPROTO_SCTP!\n");
        pListenSock = new SctpSocket;
        for (int i = 0; i < MAX_CONNECTIONS; i++)
            pConnectedSocks[i] = new SctpSocket;
        pListenSock->init_local_host(&socketParams);
        fd  = pListenSock->create_socket();
        ret = pListenSock->sbind();
        if (ret < 0)
            return ret;
        // pListenSock->set_block(0);
        pListenSock->set_send_recv_buf(DATA_BUFFER, DATA_BUFFER);
        pListenSock->slisten();
        break;
    default:
        LOG_ERROR("Not support this protoType: %d\n", socketParams.protoType);
        exit(1);
    }
    return ret;
}

int SocketServer::recv(int conIdx, void* msgBuf){
    int retV = 0;
    switch (socketParams.protoType)
    {
    case IPPROTO_SCTP:
        retV = sctp_recv(conIdx, msgBuf);
        break;
    default:
        LOG_ERROR("Not support this protoType: %d", socketParams.protoType);
        exit(1);
    }
    return retV;
}

int SocketServer::recv(void** msgBuf, size_t* msgLen)
{
    int retV = 0;
    switch (socketParams.protoType)
    {
    case IPPROTO_SCTP:
        retV = sctp_recv(msgBuf, msgLen);
        break;
    default:
        LOG_ERROR("Not support this protoType: %d", socketParams.protoType);
        exit(1);
    }
    return retV;
}

int SocketServer::send(int conIdx, void* msg, size_t msg_len)
{
    if (pConnectedSocks[conIdx]->get_socket_fd() == -1)
    {
        LOG_ERROR("AL: Sock FD of connection %d is invalid\n", conIdx);
        return -1;
    }
    int rc = pConnectedSocks[conIdx]->ssend((uint8_t*)msg, msg_len);
    return rc;
}


int SocketServer::getConnectionNum(int* bmpConn)
{
    int numConn = 0;
    if (bmpConn != nullptr)
        *bmpConn = 0;
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (pConnectedSocks[i]->get_socket_fd() != -1)
        {
            if (bmpConn != nullptr)
                *bmpConn |= 1 << i;
            numConn++;
        }
    }
    return numConn;
}

int SocketServer::sctp_recv(int conIdx, void* msgBuf){
    fd_set read_fd_set;
    FD_ZERO(&read_fd_set);

    FD_SET(pListenSock->get_socket_fd(), &read_fd_set);
    int availConnIdx = -1;
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (pConnectedSocks[i]->get_socket_fd() >= 0)
        {
            FD_SET(pConnectedSocks[i]->get_socket_fd(), &read_fd_set);
        }
        else
        {
            if (availConnIdx == -1) // availConnIdx代表在pConnectedSocks中没有使用到的索引。
                availConnIdx = i;
        }
    }

    struct timeval tv;
    tv.tv_sec  = 0; 
    tv.tv_usec = 10;

    int retSelVal       = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &tv);
    int totalReadLength = 0;

    if (retSelVal > 0)
    {
        LOG_INFO("Select returned with %d\n", retSelVal);
        const int& serverFd = pListenSock->get_socket_fd();
        if (FD_ISSET(serverFd, &read_fd_set)) // 这里如果是服务器的ListenSock被select了
        {
            if (pConnectedSocks[availConnIdx]->saccept(serverFd) == false)
            {
                LOG_ERROR("accept failed [%s]\n", strerror(errno));
            }
            else
            {
                LOG_INFO("FD: %d accept a connection.\n", serverFd);
                pConnectedSocks[availConnIdx]->set_send_recv_buf(DATA_BUFFER, DATA_BUFFER);
            }
        }


        for (int i = 0; i < MAX_CONNECTIONS; i++)
        {
            if(i != conIdx)
                continue;
            if ((pConnectedSocks[i]->get_socket_fd() > 0) && (FD_ISSET(pConnectedSocks[i]->get_socket_fd(), &read_fd_set)))
            {
                LOG_INFO("Returned fd is %d [index, i: %d]\n", pConnectedSocks[i]->get_socket_fd(), i);
                int retReadVal;
                retReadVal = pConnectedSocks[i]->srecv(msgBuf, DATA_BUFFER);
                if (retReadVal == 0)
                {
                    LOG_ERROR("Closing connection for fd:%d\n", pConnectedSocks[i]->get_socket_fd());
                    pConnectedSocks[i]->sclose();
                }
                if (retReadVal > 0)
                {
                    LOG_INFO("AL Received msg from sockFD:%d (%s:%d), length:%d. bytes:\n %*.s\n",
                                pConnectedSocks[i]->get_socket_fd(), pConnectedSocks[i]->getHostName(false),
                                pConnectedSocks[i]->getPort(false), retReadVal, 10,
                                (char*)msgBuf);  // print first 10 charactors
                    totalReadLength += retReadVal;
                }
                if (retReadVal == -1)
                {
                    LOG_ERROR("AL recv() failed from sockFD:%d (%s:%d) [%s]\n", pConnectedSocks[i]->get_socket_fd(),
                                pConnectedSocks[i]->getHostName(), pConnectedSocks[i]->getPort(), strerror(errno));
                    break;
                }
            }
        }
    }
    return totalReadLength;
}


int SocketServer::sctp_recv(void** msgBuf, size_t* msgLen)
{
    // https://langzi989.github.io/2017/10/08/Unix中SelectPollEpoll详解/ 多路复用IO

    fd_set read_fd_set;
    FD_ZERO(&read_fd_set);

    FD_SET(pListenSock->get_socket_fd(), &read_fd_set);
    int availConnIdx = -1;
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (pConnectedSocks[i]->get_socket_fd() >= 0)
        {
            FD_SET(pConnectedSocks[i]->get_socket_fd(), &read_fd_set);
        }
        else
        {
            if (availConnIdx == -1) // availConnIdx代表在pConnectedSocks中没有使用到的索引。
                availConnIdx = i;
        }
    }

    struct timeval tv;
    tv.tv_sec  = 0; 
    tv.tv_usec = 10;

    int retSelVal       = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &tv);
    int totalReadLength = 0;

    if (retSelVal > 0)
    {
        LOG_INFO("Select returned with %d\n", retSelVal);
        const int& serverFd = pListenSock->get_socket_fd();
        if (FD_ISSET(serverFd, &read_fd_set)) // 这里如果是服务器的ListenSock被select了
        {
            if (pConnectedSocks[availConnIdx]->saccept(serverFd) == false)
            {
                LOG_ERROR("accept failed [%s]. availConnIdx=%d, serverFd=%d\n", strerror(errno), 
                availConnIdx, serverFd);
            }
            else
            {
                LOG_INFO("FD: %d accept a connection.\n", serverFd);
                pConnectedSocks[availConnIdx]->set_send_recv_buf(DATA_BUFFER, DATA_BUFFER);
            }
        }


        for (int i = 0; i < MAX_CONNECTIONS; i++)
        {
            msgLen[i] = 0;
            if ((pConnectedSocks[i]->get_socket_fd() > 0) && (FD_ISSET(pConnectedSocks[i]->get_socket_fd(), &read_fd_set)))
            {
                LOG_INFO("Returned fd is %d [index, i: %d]\n", pConnectedSocks[i]->get_socket_fd(), i);
                int retReadVal;
                retReadVal = pConnectedSocks[i]->srecv(msgBuf[i], DATA_BUFFER);
                if (retReadVal == 0)
                {
                    LOG_ERROR("Closing connection for fd:%d\n", pConnectedSocks[i]->get_socket_fd());
                    pConnectedSocks[i]->sclose();
                }
                if (retReadVal > 0)
                {
                    LOG_INFO("AL Received msg from sockFD:%d (%s:%d), length:%d. bytes:\n %*.s\n",
                                pConnectedSocks[i]->get_socket_fd(), pConnectedSocks[i]->getHostName(false),
                                pConnectedSocks[i]->getPort(false), retReadVal, 10,
                                (char*)msgBuf[i]);  // print first 10 charactors
                    msgLen[i] = retReadVal;
                    totalReadLength += retReadVal;
                }
                if (retReadVal == -1)
                {
                    LOG_ERROR("AL recv() failed from sockFD:%d (%s:%d) [%s]\n", pConnectedSocks[i]->get_socket_fd(),
                                pConnectedSocks[i]->getHostName(), pConnectedSocks[i]->getPort(), strerror(errno));
                    break;
                }
            }
        }
    }
    return totalReadLength;
}


