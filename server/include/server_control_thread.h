#ifndef SERVER_CONTROL_THREAD_H
#define SERVER_CONTROL_THREAD_H

#include "SocketServer.h"
#include "Socket.h"
#include "CommonDefs.h"

int ctrl_msg_hdlr(char* msg, size_t len, int conId);
void* ctrl_thread(void*);

#endif