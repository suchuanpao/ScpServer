/*************************************************************************
	> File Name: sc_mutex.c
	> Author: chuanpao su
	> Mail: 291657954@qq.com
	> Created Time: Sat 23 Apr 2016 10:08:06 AM CST
 ************************************************************************/
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "sc_lock.h"
#include "sc_mutex.h"

static int Sc_MutexLock(ScLockInfo *st_lock_info)
{
    ScMutexInfo * st_mutex_info = st_lock_info->priv;
    assert(st_mutex_info != NULL);
    return pthread_mutex_lock(&st_mutex_info->mutex);
}

static int Sc_MutexUnlock(ScLockInfo * st_lock_info)
{
    ScMutexInfo * st_mutex_info = st_lock_info->priv;    
    assert(st_mutex_info != NULL);
    return pthread_mutex_unlock(&st_mutex_info->mutex);
}

static int Sc_MutexTrylock(ScLockInfo * st_lock_info)
{
    ScMutexInfo * st_mutex_info = st_lock_info->priv;    
    assert(st_mutex_info != NULL);
    return pthread_mutex_lock(&st_mutex_info->mutex);
}

static int Sc_controlMutex(ScLockInfo * st_lock_info, int ctl, va_list pArgs)
{
    int ret, param;
    ScMutexInfo * st_mutex_info = st_lock_info->priv;
    assert(st_mutex_info != NULL);
    pthread_mutex_t * mutex = &st_mutex_info->mutex;
    pthread_mutexattr_t * mutex_attr = &st_mutex_info->mutex_attr;
    
    switch(ctl) {
        //设置/获取 线程锁类型
        case SC_MUTEX_GET_TYPE:
            break;
        case SC_MUTEX_SET_TYPE:
            ret |= pthread_mutexattr_settype(mutex_attr,ctl);
            break;
        //设置/获取 线程锁范围
        case SC_MUTEX_GET_PROCESS:
            break;
        case SC_MUTEX_SET_PROCESS:
            param = va_arg(pArgs,int);
            ret |= pthread_mutexattr_setpshared(mutex_attr,param);
            break;
        //设置/获取 线程锁协议
        case SC_MUTEX_GET_PROTOCAL:
            break;
        case SC_MUTEX_SET_PROTOCAL:
            break;
    }
    return ret?SC_LOCK_FAILED:SC_LOCK_SUCCESS;
}

static int Sc_registerMutex(ScLockInfo * st_lock_info)
{
    ScMutexInfo * st_mutex_info = (ScMutexInfo*)st_lock_info->priv;
    assert(st_mutex_info != NULL);
    return pthread_mutex_init(&st_mutex_info->mutex, \
                      &st_mutex_info->mutex_attr);

}
static int Sc_unregisterMutex(ScLockInfo * st_lock_info)
{
    int ret = 0;
    ScMutexInfo * st_mutex_info = (ScMutexInfo*)st_lock_info->priv;
    assert(st_mutex_info != NULL);
    ret |= pthread_mutexattr_destroy(&st_mutex_info->mutex_attr);
    ret |= pthread_mutex_destroy(&st_mutex_info->mutex);
    return ret?SC_LOCK_FAILED:SC_LOCK_SUCCESS;
}

static int Sc_createMutex(ScLockInfo * st_lock_info)
{
    ScMutexInfo * st_mutex_info = (ScMutexInfo*)malloc( sizeof(ScMutexInfo) );
    if(!st_mutex_info){
        return SC_LOCK_FAILED;
    }
    memset(st_mutex_info, 0, sizeof(ScMutexInfo));
    //初始化线程锁的属性,该属性全程有效
    pthread_mutexattr_init(&st_mutex_info->mutex_attr);
    st_lock_info->priv = st_mutex_info;
    return SC_LOCK_SUCCESS;
}

ScLockOps mutexOps = {
    .unlock = Sc_MutexUnlock,
    .lock = Sc_MutexLock,
    .trylock = Sc_MutexTrylock,
    .controlLock = Sc_controlMutex,
    .setLockType = Sc_createMutex,
    .registerLock = Sc_registerMutex,
    .unregisterLock = Sc_unregisterMutex,
};
