/*************************************************************************
	> File Name: sc_lock.h
	> Author: chuanpao su
	> Mail: 291657954@qq.com
	> Created Time: Thu 21 Apr 2016 07:30:48 PM CST
 ************************************************************************/

#ifndef _SC_LOCK_H
#define _SC_LOCK_H

#include <stdarg.h>



typedef enum
{
    SC_LOCK_FAILED = -1,
    SC_LOCK_SUCCESS = 0,
} ScLockRet;


typedef enum
{
    //线程锁，可设置为进程锁
    SC_MUTEX = 0,
    //读写锁
    SC_RWLOCK,
    SC_SEMLOCK,
    SC_RECLOCK,
} ScLockType;



typedef struct __ScLockInfo
{
    char * pfile;
    void * priv;
} ScLockInfo;

typedef struct __ScLockOps
{
    int (*lock)(ScLockInfo *);
    int (*unlock)(ScLockInfo *);
    int (*trylock)(ScLockInfo *);
    int (*controlLock)(ScLockInfo *, int, va_list);
    int (*setLockType)(ScLockInfo *);
    int (*registerLock)(ScLockInfo *);
    int (*unregisterLock)(ScLockInfo *);
} ScLockOps;
/*
 * The Lock Info(ScLock) and Lock operations(ScLockOps) be separated 
 * is helpful to make locks modular which attributes and operations is 
 * diffirent ;
 * */
typedef struct __ScLock
{
    ScLockInfo lock_info;
    ScLockOps *ops;
    int (*lock)(struct __ScLock *);
    int (*unlock)(struct __ScLock *);
    int (*trylock)(struct __ScLock *);
    int (*controlLock)(struct __ScLock *, int, ...);
    int (*setLockType)(struct __ScLock *, ScLockType);
    int (*registerLock)(struct __ScLock *);
    int (*unregisterLock)(struct __ScLock *);
} ScLock;


extern ScLock *Sc_createLock();
extern int Sc_initLock(ScLock * this);
extern void Sc_releaseLock(ScLock * this);

#endif
