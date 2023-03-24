/*
 * AdaptLayerIpcSocketServer.h
 */

#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_

#include "AbstractServer.h"
#include "Socket.h"

class SocketServer: public AbstractServer
{
public:
    SocketParamStruct           socketParams;

    SocketServer();
    virtual ~SocketServer();
    virtual int create() override;
    virtual int send(int conIdx, void* msg, size_t msg_len) override;
    virtual int recv(void** msgBuf, size_t* msgLen) override;
    virtual int recv(int conIdx, void* msgBuf) override;
    virtual int getConnectionNum(int* bmpConn = nullptr) override;

    virtual int getMaxConnections() override
    {
        return MAX_CONNECTIONS;
    }
    virtual void close() override
    {
        if (pListenSock && pListenSock->get_socket_fd() > 0)
        {
            pListenSock->sclose();
        }
        for (int i = 0; i < MAX_CONNECTIONS; i++)
        {
            if (pConnectedSocks[i] && pConnectedSocks[i]->get_socket_fd() > 0)
                pConnectedSocks[i]->sclose();
        }
    };

private: 
    Socket*                     pListenSock{nullptr};
    Socket*                     pConnectedSocks[MAX_CONNECTIONS];
    bool                        nonBlockListen{false};  

private: 
    int sctp_recv(void** msgBuf, size_t* msgLen);
    int sctp_recv(int conIdx, void* msgBuf);
};

#endif
