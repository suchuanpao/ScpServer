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

typedef struct __ErrorMsgHead{
    ListHead head;
    pthread_mutex_t;
}ErrorMsgHead;

typedef struct __ErrorMsg{
    int err_no;     
    pthread_t tid;
    ListHead list;
    char err_string[MAX_ERROR_STRING_LENGTH];
    char err_reason[MAX_ERROR_STRING_LENGTH];
}ErrorMsg;

typedef enum
{
    SC_ERROR_MY_FAILED = -2,
    SC_ERROR_FAILED = -1,
    SC_ERROR_SUCCESS = 0,
    SC_ERROR_WARN = 1,
}ScErrorType

#endif
