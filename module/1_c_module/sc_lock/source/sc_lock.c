/*************************************************************************
	> File Name: sc_lock.c
	> Author: chuanpao su
	> Mail: 291657954@qq.com
	> Created Time: Thu 21 Apr 2016 08:31:02 PM CST
 ************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include "sc_lock.h"
#include "sc_mutex.h"

static int Sc_lock(ScLock * this)
{
    assert(this && this->ops);
    return this->ops->lock(&this->lock_info);
}

static int Sc_unlock(ScLock * this)
{
    assert(this && this->ops);
    return this->ops->unlock(&this->lock_info);
}

static int Sc_trylock(ScLock * this)
{
    assert(this && this->ops);
    return this->ops->trylock(&this->lock_info);
}

static int Sc_setLockType(ScLock * this, ScLockType en_type)
{
    assert(this);
    switch (en_type)
    {
        case SC_MUTEX:
            this->ops = &mutexOps;
            break;
        case SC_RWLOCK:
            break;
    }

    return this->ops->setLockType(&this->lock_info);
}

static int Sc_controlLock(ScLock * this, int attr, ...)
{
    assert(this && this->ops);
    ScLockRet ret;

    va_list pArgs;
    va_start(pArgs,attr);

    ret = this->ops->controlLock(&this->lock_info, attr, pArgs);

    va_end(pArgs);

    return ret;
}

static int Sc_registerLock(ScLock * this)
{
    assert(this && this->ops);
    return this->ops->registerLock(&this->lock_info);
}

ScLock * Sc_createLock()
{
    ScLock * this = (ScLock * )malloc(sizeof(ScLock));

    if (!this) {
        return NULL;
    }

    if (Sc_initLock(this) != SC_LOCK_SUCCESS) {
        Sc_releaseLock(this);
        this = NULL;
    }

    return this;
}

int Sc_initLock(ScLock * this)
{
    assert(this);
    memset(this, 0, sizeof(ScLock));

    this->lock = Sc_lock;
    this->trylock = Sc_trylock;
    this->unlock = Sc_unlock;
    this->setLockType = Sc_setLockType;
    this->controlLock = Sc_controlLock;
    this->registerLock = Sc_registerLock;
}

void Sc_releaseLock(ScLock * this)
{
    assert(this);
    memset(this,0,sizeof(ScLock));
    free(this);
}
