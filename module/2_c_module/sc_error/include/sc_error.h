/*************************************************************************
	> File Name: sc_error.h
	> Author: chuanpao su
	> Mail: 291657954@qq.com
	> Created Time: Sun 10 Apr 2016 04:00:51 PM CST
 ************************************************************************/

#include <pthread.h>
#include "Sc/list.h"
#ifndef _SC_ERROR_H
#define _SC_ERROR_H


#define MAX_ERROR_STRING_LENGTH 1024
#define SC_THREAD_T pthread_t
#define SC_THREAD_MUTEX pthread_mutex_t

typedef enum
{
    SC_ERROR_MY_FAILED = -2,
    SC_ERROR_FAILED = -1,
    SC_ERROR_SUCCESS = 0,
    SC_ERROR_WARN = 1,
}ScSetErrorType

typedef enum
{   
    SC_ERROR_NO = 0,
    SC_ERROR_MESSAGE = 1,
    SC_ERROR_POS = 2,
    SC_ERROR_FOLLOW = 3,
    SC_ERROR_WARN = 4,
}ScGetErrorType


typedef __ScErrorHead
{
    ListHead head;
    SC_THREAD_T mutex;
}ScErrorHead;

typedef __ScError
{
    int errno;
    SC_THREAD_T tid;
    char error_pos[MAX_ERROR_STRING_LENGTH];
    char error_Message[MAX_ERROR_STRING_LENGTH];
    char error_result[MAX_ERROR_STRING_LENGTH];
    ListHead list;
}ScError;

char * Sc_getError();

void Sc_setError(ScErrorType errno ,char * errstr);

#define SC_SET_ERROR(errno, errstr,...) \
    Sc_setError(errno, errstr, ...)
    

#endif
