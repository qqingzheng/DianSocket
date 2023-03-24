#ifndef SERVER_CONTROL_THREAD_H
#define SERVER_CONTROL_THREAD_H

#include "SocketServer.h"
#include "Socket.h"
#include "CommonDefs.h"
#include <string.h>

int ctrl_msg_hdlr(char*, size_t, int);
void* ctrl_thread(void*);
int thread_affinity_bind(int);
int thread_set_prio(int, int);

#endif