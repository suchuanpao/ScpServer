/*************************************************************************
	> File Name: sc_mutex.h
	> Author: chuanpao su
	> Mail: 291657954@qq.com
	> Created Time: Sat 23 Apr 2016 09:47:52 AM CST
 ************************************************************************/

#ifndef _SC_MUTEX_H
#define _SC_MUTEX_H
#include <pthread.h>
#include "sc_lock.h"


typedef struct __ScMutexInfo
{
    pthread_mutex_t mutex;
    pthread_mutexattr_t *mutex_attr;
} ScMutexInfo;


typedef enum {
//设置/获取 线程锁类型
    SC_MUTEX_GET_TYPE,
    SC_MUTEX_SET_TYPE,
//设置/获取 线程锁范围
    SC_MUTEX_GET_PROCESS,
    SC_MUTEX_SET_PROCESS,
//设置/获取 线程锁协议
    SC_MUTEX_GET_PROTOCAL,
    SC_MUTEX_SET_PROTOCAL,
} ScLockCtl;

typedef enum
{           
    //多进程锁
    SC_MUTEX_PROCESS_SHARED = 0,
    //单进程锁
    SC_MUTEX_PROCESS_PRIVATE,
    //重复锁定会导致死锁
    SC_MUTEX_TYPE_NORMAL,
    //自动检测死锁，只提供错误检查，重复锁定有错误
    SC_MUTEX_TYPE_ERRORCHECK,
    //技术锁定次数，使用这个属性，上锁的线程，必须由该线程解锁
    SC_MUTEX_TYPE_RESCURSIVE,
    //重复锁定会导致不可预料的后果
    SC_MUTEX_TYPE_DEFAULT,
    //线程优先级和调度不会收到互斥量的影响
    SC_MUTEX_PROTOCOL_NONE,
    //
} ScLockAttr;

extern ScLockOps mutexOps;




#endif
