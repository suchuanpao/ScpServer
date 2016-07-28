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
/* Sc_lock:
 * @param: Sc_Lock
 * notice : Locker must be register in Sc_registerLock.
 * To lock Locker.
* */
static int Sc_lock(ScLock * this)
{
    assert(this && this->ops);
    assert(this->ops->lock);
    return this->ops->lock(&this->lock_info);
}
/* Sc_unlock:2
 * @param: Sc_unLock
 * notice : Locker must be register in Sc_registerLock.
 * To unlock Locker.
* */

static int Sc_unlock(ScLock * this)
{
    assert(this && this->ops);
    assert(this->ops->unlock);
    return this->ops->unlock(&this->lock_info);
}
/* Sc_trylock:
 * @param: Sc_Lock
 * notice : Locker must be register in Sc_registerLock.
 * Try lock Locker.
* */

static int Sc_trylock(ScLock * this)
{
    assert(this && this->ops);
    assert(this->ops->trylock);
    return this->ops->trylock(&this->lock_info);
}
/* Sc_setLockType:
 * @param 1: Sc_Lock
 * @param 2: ScLockType
 * notice : Locker must be initialized in @Sc_createLock or @Sc_initLock.
 * If Locker only initialized, it will have to set type to choose
 * which Locker you want, so you should use Sc_setLockType function.
 * you can see Locker Type which you can choose in ScLockType constant.
* */

static int Sc_setLockType(ScLock * this, ScLockType en_type)
{
    assert(this);
    assert(this->ops->setLockType);
    switch (en_type)
    {
        case SC_MUTEX:
            this->ops = &mutexOps;
            break;
        case SC_RWLOCK:
            break;
        case SC_SEMLOCK:
            break;
        case SC_RECLOCK:
            break;
    }
    
    return this->ops->setLockType(&this->lock_info);
}

/* Sc_controlLock:
 * @param 1: Sc_Lock
 * @param 2: int attr
 * @param 3: (...)
 * notice : Locker must be initialized, and this function should be
 * used before @Sc_setLockType function.
 * You can control Locker's attributes in Sc_controlLock function.
 * If you set thread_lock(Muetx) type of Locker, then you should set
 * SC_MUTEX_ATTR beginning of the parameter attr's value.
* */
static int Sc_controlLock(ScLock * this, int ctl, ...)
{
    assert(this && this->ops);
    assert(this->ops->controlLock);
    ScLockRet ret;
    va_list pArgs;
    va_start(pArgs,ctl);
    ret = this->ops->controlLock(&this->lock_info, ctl, pArgs);
    va_end(pArgs);
    return ret;
}
/* Sc_registerLock:
 * @param: Sc_Lock
 * notice : Locker must be register in Sc_registerLock.
 * Register Locker. If not set type or attribute of Lock, it will be set
 * SC_MUTEX Locker.
* */

static int Sc_registerLock(ScLock * this)
{
    assert(this && this->ops);
    assert(this->ops->registerLock);
    return this->ops->registerLock(&this->lock_info);
}
/* Sc_unregisterLock:
 * @param: Sc_Lock
 * notice : Locker must be register in Sc_registerLock.
 * Unegister Locker. If not set type or attribute of Lock, The Locker must be 
 * register, or it will not pass.
* */
static int Sc_unregisterLock(ScLock * this)
{
    assert(this && this->ops);
    assert(this->ops->unregisterLock);
    return this->ops->unregisterLock(&this->lock_info);
}

/* Sc_createLock:
 * @param: Sc_Lock
 * Try lock Locker.
 * Malloc memory in itself process space, and get operations.
* */
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

/* Sc_initLock:
 * @param: Sc_Lock
 * Init Locker, and get operations.
* */
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
    this->unregisterLock = Sc_unregisterLock;
}
/* Sc_releaseLock:
 * @param: Sc_Lock
 * Release Locker.
* */
void Sc_releaseLock(ScLock * this)
{
    assert(this);
    memset(this,0,sizeof(ScLock));
    free(this);
}
