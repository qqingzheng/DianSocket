#ifndef SCTPSOCKET_H_
#define SCTPSOCKET_H_
#include "Socket.h"
#include <unistd.h>
#include <netinet/sctp.h>
#include <tuple>

#define SCTP_MAX_RETRY 5

class SctpSocket : public Socket{
    public:
    SctpSocket() { };
    virtual ~SctpSocket() {};

    int32_t                  create_socket();
    bool                     saccept(int32_t listenSockFd) override;
    int                      slisten() override;                        
    int32_t                  sconnect() override;                       
    ssize_t                  ssend(void* msg, size_t msg_len) override;
    ssize_t                  srecv(void* recvbuf, size_t buflen) override;
private:
    typedef struct
    {
        struct sockaddr* conn_x_addrs;
        int32_t          connectx_count;
    } sCfgSocketSctpAddings;

    sCfgSocketSctpAddings sockCfg_sctpAdding;

private:
    void setNotifyEvents(int32_t sockFd);
    void handleNotifications(void* recvBuf);
    void print_ctrlmsg(int type, sctp_cmsg_data_t* data);
};


#endif