/*************************************************************************
	> File Name: sc_mutex.c
	> Author: chuanpao su
	> Mail: 291657954@qq.com
	> Created Time: Sat 23 Apr 2016 10:08:06 AM CST
 ************************************************************************/

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "sc_lock.h"
#include "sc_mutex.h"

static int Sc_MutexLock(ScLockInfo *st_lock_info)
{
    ScMutexInfo * st_mutex_info = st_lock_info->priv;    
    return pthread_mutex_lock(&st_mutex_info->mutex);
}

static int Sc_MutexUnlock(ScLockInfo * st_lock_info)
{
    ScMutexInfo * st_mutex_info = st_lock_info->priv;    
    return pthread_mutex_lock(&st_mutex_info->mutex);
}

static int Sc_MutexTrylock(ScLockInfo * st_lock_info)
{
    ScMutexInfo * st_mutex_info = st_lock_info->priv;    
    return pthread_mutex_lock(&st_mutex_info->mutex);
}

static int Sc_controlMutex(ScLockInfo * st_lock_info, int attr, va_list pArgs)
{
    int ret;

    ScMutexInfo * st_mutex_info = st_lock_info->priv;

    st_mutex_info->mutex_attr = (pthread_mutexattr_t *)malloc( sizeof(*(st_mutex_info->mutex_attr)) );

    if(!st_mutex_info->mutex_attr) {
        return SC_LOCK_FAILED;    
    }

    pthread_mutex_t * mutex = &st_mutex_info->mutex;

    pthread_mutexattr_t * mutex_attr = st_mutex_info->mutex_attr;

    pthread_mutexattr_init(mutex_attr);

    switch(attr) {
        case SC_MUTEX_PROCESS_SHARED:
            ret = pthread_mutexattr_setpshared(mutex_attr, PTHREAD_PROCESS_SHARED);
            break;
        case SC_MUTEX_TYPE_NORMAL:
        case SC_MUTEX_TYPE_ERRORCHECK:
        case SC_MUTEX_TYPE_RESCURSIVE:
        case SC_MUTEX_TYPE_DEFAULT:
            ret = pthread_mutexattr_settype(mutex_attr,attr);
            break;
    }
    return SC_LOCK_SUCCESS;
}

static int Sc_registerMutex(ScLockInfo * st_lock_info)
{
    ScMutexInfo * st_mutex_info = st_lock_info->priv;

    return pthread_mutex_init(&st_mutex_info->mutex, \
                      st_mutex_info->mutex_attr);

}

static int Sc_createMutex(ScLockInfo * st_lock_info)
{
    ScMutexInfo * st_mutex_info = (ScMutexInfo*)malloc( sizeof(ScMutexInfo) );
    if(!st_mutex_info){
        return SC_LOCK_FAILED;
    }
    memset(st_mutex_info, 0, sizeof(ScMutexInfo));

    st_lock_info->priv = st_mutex_info;

    return SC_LOCK_SUCCESS;
}

ScLockOps mutexOps = {
    .lock = Sc_MutexLock,
    .trylock = Sc_MutexTrylock,
    .unlock = Sc_MutexUnlock,
    .controlLock = Sc_controlMutex,
    .setLockType = Sc_createMutex,
    .registerLock = Sc_registerMutex,
};
