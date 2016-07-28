/*************************************************************************
	> File Name: sc_thread.h
	> Author: chuanpao su
	> Mail: 291657954@qq.com
	> Created Time: 2016年04月19日 星期二 10时47分22秒
 ************************************************************************/

#ifndef _SC_THREAD_H
#define _SC_THREAD_H


#include <pthread.h>

typedef enum{
    
}ScThreadAttrType;

typedef struct __ScThread
{
    
}ScThread;

thread_t ScGetThreadId();

thread_t ScCreateThread(ScThreadAttrType en_attr, \
                        size_t stack_size, \
                        void *(*start_routine)(void *), void *);



#endif
