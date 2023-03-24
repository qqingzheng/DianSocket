#include "SctpSocket.h"
#include <cstdio>
#include <string.h>
#include <pthread.h>
using namespace std;

int main(int argc, const char** argv){
    if(argc < 3){
        LOG_ERROR("Not enough arguments input...")
        return 1;
    }
    SctpSocket socket;
    SocketParamStruct sSocketParam;
    strcpy(sSocketParam.HostName, argv[1]);
    sSocketParam.Port = atoi(argv[2]);
    socket.init_remote_host(&sSocketParam);
    socket.create_socket();
    if(socket.sconnect()){
        LOG_ERROR("connect to server %s:%d failed.", sSocketParam.HostName, sSocketParam.Port);
        return 1;
    }
    char buf[4] = "asd";
    socket.ssend(buf, 3);
    return 0;
}