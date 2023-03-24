#include "server_control_thread.h"
#include <cstdio>
#include <string.h>
#include <pthread.h>
using namespace std;

int main(int argc, const char** argv){
    
    if(argc < 5){
        LOG_ERROR("Not enough arguments input...")
        return 1;
    }

    // 主线程绑定核
    thread_affinity_bind(atoi(argv[1]));


    // 基础控制线程，用于管理基础请求。
    pthread_t controlThread;
    if (int rc = pthread_create(&controlThread, NULL, ctrl_thread, argv+2))
    {
        LOG_INFO("failed to create ctrl thread: %d\n", rc);
        return 1;
    }

    pthread_join(controlThread, NULL);
    return 0;
}