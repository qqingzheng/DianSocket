#include "server_control_thread.h"

void* ctrl_thread(void* argv){
    SocketServer ctrlSocketServer;

    int sockFd = ctrlSocketServer.create();
    if(sockFd < 0){
        LOG_ERROR("Create listening Socket failed!");
        return NULL;
    }
    LOG_INFO("Listening sockFd = %d", sockFd);

    int           rcv          = 0;
    void*         msgBuf[MAX_CONNECTIONS];
    for (int idx = 0; idx < MAX_CONNECTIONS; ++idx)
        msgBuf[idx] = malloc(DATA_BUFFER);
    size_t rcvMsgLen[MAX_CONNECTIONS];

    while (1)
    {
        do{
            rcv = ctrlSocketServer.recv((void**)msgBuf, rcvMsgLen);
            usleep(100);  // 暂停100us
        }while(rcv <= 0); // 循环，一直到接收到消息
        
        for (int conIdx = 0; conIdx < MAX_CONNECTIONS; conIdx++)
        {
            if (rcvMsgLen[conIdx] == 0)
                continue;
            if (ctrl_msg_hdlr((char*)msgBuf[conIdx], rcvMsgLen[conIdx], conIdx) != 0)
            {
                printf("CU: failed to handle msg, continue\n");
            }
        }
    }
}

int ctrl_msg_hdlr(char* msg, size_t len, int conId){
    
}