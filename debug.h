#ifndef __DEBUG__
#define __DEBUG__

#include <stdio.h>

#define DEBUG 1

#define DEBUGE(fmt, ...)\
        do{ if(DEBUG){ printf("\033[0;31m" "ERROR: %s:%d:%s(): " #fmt "\r\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__);} }while (false);

#define DEBUGI(fmt, ...)\
        do{ if(DEBUG) {printf("\033[0;37m" "INFO: %s:%d:%s(): " #fmt "\r\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__);} }while (false);

#define ERROR_CHECK(x, error_msg)    \
{                                    \
    int error = (x);                 \
    if(error == -1){                 \
        printf("%s\r\n", error_msg); \
        exit(0);                     \
    }                                \
}

#define ERROR_CHECK_SOFT(x, error_msg)  \
{                                       \
    int error = (x);                    \
    if(error == -1){                    \
        printf("%s\r\n", error_msg);    \
        return -1;                      \
    }                                   \
}

#endif /* __DEBUG__ */