#ifndef __SCP_SOCK_H_
#define __SCP_SOCK_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>


//三大宏定义
//SCP_ERRSTR   无字符串打印等消息,测试期间，开发期间使用
//SCP_NDEBUG   assert开发期使用
//SCP_OS       //模块内部使用的返回值宏
#ifdef SCP_ERRSTR
    #define SCP_SOCK_ERRSTR
#endif

#ifdef SCP_SOCK_ERRSTR
typedef struct __ErrorMsg{
    int err_no;
    char err_string[256];
    char err_reason[256];
    char * (*err_str)(struct __ScpSockClass*);
    struct __ErrorMsg * next;
}ErrorMsg;
#endif


typedef enum __ScpSockType{
	SCP_SOCK_MY_FAILED = -2,
	SCP_SOCK_FAILED = -1,
	SCP_SOCK_SUCCESS = 0,
	SCP_SOCK_CLIENT,
	SCP_SOCK_SERVER
}ScpSockType;

//内部使用固定值
#define ADDR_LEN_MIN 64 //地址最小长度必须大于64


//客户端或者服务端类型

typedef struct __ScpSockClass{
	int sock;
	struct sockaddr_in addr;
	ScpSockType en_cli_or_ser;
#ifdef SCP_SOCK_ERRSTR
    ErrorMsg st_err_head;
#endif
//套接字初始化以及关闭
    //初始化套接字时设置为套接字重复用，使用完套接字应设成0
	int (*initSock)(struct __ScpSockClass *, ScpSockType);
	int (*initAddress)(struct __ScpSockClass*,const char *,int,int);
	int (*close)(struct __ScpSockClass*);
	int (*read)(struct __ScpSockClass*, char *, int, int);
	int (*write)(struct __ScpSockClass*, const char *, int, int);
//客户端套接字操作
	int (*connect2Server)(struct __ScpSockClass *);
//服务器操作
	int (*bind)(struct __ScpSockClass*);
	int (*listen)(struct __ScpSockClass*,int);
	struct __ScpSockClass * (*accept)(struct __ScpSockClass*);
//设置属性
    int (*setTimeout)(struct __ScpSockClass *, int,int);
    int (*setSockReuse)(struct __ScpSockClass*);
//获得属性
    int (*getAddr)(struct __ScpSockClass *, char * ,int);
    char * (*getAddrOnlyRead)(struct __ScpSockClass *);
//线程锁初始化，线程锁上锁
    int (*useByThread)(struct __ScpSockClass *,pthread_t *);
    int (*lock)(struct __ScpSockClass *);
    int (*unlock)(struct __ScpSockType *);
}ScpSockClass;



//create or init ScpSockClass
//使用初始化不需要释放
void Scp_initSockClass(ScpSockClass* this);
//使用create申请需要自己释放
ScpSockClass * Scp_createSockClass(void);
void Scp_releaseSockClass(ScpSockClass * this);




#endif
