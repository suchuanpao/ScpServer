#ifndef __SCP_SOCK_H_
#define __SCP_SOCK_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

//如果未定义日志记录错误的宏，则直接输出
#ifdef DEBUG
#define SOCK_DEBUG_PRINT(ret,err_str) printf("%s,%s,%d:return value %d.\nsysterm:%s.\n:lib:%s\n",\
                                     __FILE__,__func__,__LINE__,ret,strerror(errno),err_str)
#else
#define SOCK_DEBUG_PRINT(ret,err_str)  
#endif


//模块内部使用的返回值宏
enum SCP_SOCK_RET_VALUE{
	SCP_SOCK_RET_FAILED = -1,
	SCP_SOCK_RET_SUCCESS = 0,
	SCP_SOCK_RET_WARN = 1,
    SCP_SOCK_RET_LOG = 2
};

#define RET_VALUE SCP_SOCK_RET_VALUE
#define FAILED 	SCP_SOCK_RET_FAILED
#define SUCCESS SCP_SOCK_RET_SUCCESS
#define WARN 	SCP_SOCK_RET_WARN


//内部使用固定值
#define ADDR_LEN_MIN 64 //地址最小长度必须大于64


//客户端或者服务端类型
typedef enum __ScpSockType{
	SCP_SOCK_NONE = -1,
	SCP_SOCK_CLIENT ,
	SCP_SOCK_SERVER
}ScpSockType;

typedef struct __ScpSockClass{
	int sock;
	struct sockaddr_in addr;
	ScpSockType en_cli_or_ser;
//套接字初始化以及关闭
    //初始化套接字时设置为套接字重复用，使用完套接字应设成0
	int (*initSock)(struct __ScpSockClass *, ScpSockType);
	int (*initAddress)(struct __ScpSockClass*,const char *,int,int);
	int (*close)(struct __ScpSockClass*);
//套接字的操作
	int (*read)(struct __ScpSockClass*, char *, int, int);
	int (*write)(struct __ScpSockClass*, const char *, int, int);
//套接字操作
	int (*connect2Server)(struct __ScpSockClass *);
//服务器操作
	int (*bind)(struct __ScpSockClass*);
	int (*listen)(struct __ScpSockClass*,int);
	struct __ScpSockClass * (*accept)(struct __ScpSockClass*); 
//设置属性
    int (*setTimeout)(struct __ScpSockClass *, int);
    int (*setSockReuse)(struct __ScpSockClass*);
//获得属性
    int (*getAddr)(struct __ScpSockClass *, char * ,int);
    char * (*getAddrOnlyRead)(struct __ScpSockClass *);
}ScpSockClass;



//create or init ScpSockClass
//使用初始化不需要释放
void Scp_initSockClass(ScpSockClass* this);
//使用create申请需要自己释放
ScpSockClass * Scp_createSockClass(void);
void Scp_releaseSockClass(ScpSockClass * this);




#endif
