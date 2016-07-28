/*************************************************************************
	> File Name: sc_RW_.c
	> Author: Chuanpao Su
	> Mail: 291657954@qq.com
	> Created Time: Sun 15 May 2016 07:40:03 PM PDT
 ************************************************************************/
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "sc_lock.h"

static int Sc_RW_Lock(ScLockInfo *st_lock_info)
{
}

static int Sc_RW_Unlock(ScLockInfo * st_lock_info)
{

}

static int Sc_RW_Trylock(ScLockInfo * st_lock_info)
{

}

static int Sc_control_RW(ScLockInfo * st_lock_info, int ctl, va_list pArgs)
{

}

static int Sc_register_RW(ScLockInfo * st_lock_info)
{

}
static int Sc_unregister_RW(ScLockInfo * st_lock_info)
{

}

static int Sc_create_RW(ScLockInfo * st_lock_info)
{
    return 0;
}
ScLockOps RW_Ops = {
    .lock = Sc_RW_Lock,
    .trylock = Sc_RW_Trylock,
    .unlock = Sc_RW_Unlock,
    .controlLock = Sc_control_RW,
    .setLockType = Sc_create_RW,
    .registerLock = Sc_register_RW,
    .unregisterLock = Sc_unregister_RW,
};
