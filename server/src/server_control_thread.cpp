#include "server_control_thread.h"

void* ctrl_thread(void* _argv){
    const char** argv = (const char**) _argv;
    thread_affinity_bind(atoi(argv[0]));
    SocketServer ctrlSocketServer;
    memcpy(ctrlSocketServer.socketParams.HostName, argv[1], strlen(argv[1]));
    ctrlSocketServer.socketParams.Port = atoi(argv[2]);
    int ret = ctrlSocketServer.create();
    if(ret){
        LOG_ERROR("Create listening Socket failed!\n");
        return NULL;
    }

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
    return NULL;
}

int ctrl_msg_hdlr(char* msg, size_t len, int conId){
    int rc = 0;
    LOG_INFO("get msg");
    return rc;
}

/**
 * @brief 该函数用于线程绑核。
*/
int thread_affinity_bind(int coreNum){
    cpu_set_t cpuset;       // 一个bitmap
    int       i, rc;        // rc为返回码（return code），用于返回状态。

    CPU_ZERO(&cpuset);       // 这个宏用来将cpuset这个bitmap置0
    CPU_SET(coreNum, &cpuset);

    rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (rc) {
        perror("pthread_setaffinity_np failed");
        LOG_ERROR("pthread_setaffinity_np failed: %d\n", rc)
    }

    CPU_ZERO(&cpuset);

    rc = pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset); // 获取当前的线程绑的核

    if (rc) {
        perror("pthread_getaffinity_np failed");
        LOG_ERROR("pthread_getaffinity_np failed: %d\n", rc);
    }

    LOG_INFO("set thread %lu sys affinity: \n", pthread_self());
    for (i = 0; i < CPU_SETSIZE; i++)
        if (CPU_ISSET(i, &cpuset))
            LOG_INFO("    CPU %d\n", i);
    if (!CPU_ISSET(coreNum, &cpuset)) {
        LOG_ERROR("affinity failed\n");
    }
    return rc;
}

int thread_set_prio(int prio, int policy){
    int rc = 0;
    if (prio) {
        struct sched_param sched_param;
        sched_param.sched_priority = prio;
        if ((rc = pthread_setschedparam(pthread_self(), policy, &sched_param))) {
            LOG_ERROR("priority is not changed: %d\n", rc);
            return rc;
        }
    }
    return rc;
}