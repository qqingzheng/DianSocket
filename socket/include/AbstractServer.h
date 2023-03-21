#ifndef ABSTACTSERVER_H_
#define ABSTACTSERVER_H_
#include "CommonDefs.h"

class AbstractServer
{
public:
    AbstractServer();
    virtual ~AbstractServer();
    virtual int  create()                                    = 0;
    virtual int  send(int conIdx, void* msg, size_t msg_len) = 0;
    virtual int  recv(void** msgBuf, size_t* msgLen)         = 0;
    virtual int  recv(int conIdx, void* msgBuf)              = 0;
    virtual int  getConnectionNum(int* bmpConn = nullptr)    = 0;
    virtual int  getMaxConnections()                         = 0;
    virtual void close()
    {
        return;
    };
};

#endif
