#include "server_control_thread.h"
#include <cstdio>
#include <string.h>
#include <pthread.h>
using namespace std;

/**
 * @brief 该函数用于主线程绑核。
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

    LOG_INFO("set sys affinity: \n");
    for (i = 0; i < CPU_SETSIZE; i++)
        if (CPU_ISSET(i, &cpuset))
            LOG_INFO("    CPU %d\n", i);
    if (!CPU_ISSET(coreNum, &cpuset)) {
        LOG_ERROR("affinity failed\n");
    }
    return rc;
}

int thread_set_prio(int prio, int policy)
{
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

int main(int argc, const char** argv){
    
    // 主线程绑定核
    thread_affinity_bind(atoi(argv[1]));


    // 基础控制线程，用于管理基础请求。
    pthread_t controlThread;
    if (int rc = pthread_create(&controlThread, NULL, ctrl_thread, NULL))
    {
        LOG_INFO("failed to create ctrl thread: %d\n", rc);
        return 1;
    }
    return 0;
}