#ifndef COMMONDEFS_H_
#define COMMONDEFS_H_
#include <stddef.h>
#define DATA_BUFFER         2000000  // 2M bytes
#define MAX_CONNECTIONS     5

typedef __INT32_TYPE__  int32_t;
typedef __UINT8_TYPE__  uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;

#ifndef DEBUG
#define DEBUG 0
#endif
#define LOG_INFO(print_this...) if(DEBUG){ fprintf(stderr, "INFO:" print_this); fflush(stderr); }
#define LOG_ERROR(print_this...) if(DEBUG){ fprintf(stderr, "ERROR:" print_this); fflush(stderr);  }

#endif