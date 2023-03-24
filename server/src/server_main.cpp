#include "SocketServer.h"
#include "Socket.h"
#include <cstdio>
#include <string.h>
using namespace std;

int main(){
    SocketServer socketServer;
    SocketParamStruct& serverParam = socketServer.socketParams;
    
    memcpy(serverParam.HostName, "127.0.0.1", 10);
    serverParam.Port = 10001;
    socketServer.create();
    while(true){
    }
    return 0;
}