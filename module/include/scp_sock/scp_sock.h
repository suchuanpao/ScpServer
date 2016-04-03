#ifndef __SCP_SOCK_H_
#define __SCP_SOCK_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
/* According to POSIX.1-2001 */
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
//#include <scp_msg.h>
//如果未定义日志记录错误的宏，则直接输出
#ifndef LOG_ERR
#define SCP_PERROR(errno,err_str) printf("%s,%s,%d:%s\n",__FILE__,__func__,__LINE__,err_str)
#endif
//
enum SCP_SOCK_RET_VALUE{
	SCP_SOCK_RET_FAILED,
	SCP_SOCK_RET_SUCCESS,
	SCP_SOCK_RET_WRONG
};

//返回值全局统一则使用SCP_RET_VALUE作为返回值
#ifndef GLOBAL_ERR_CORE
#define RET_VALUE SCP_SOCK_RET_VALUE
#define FAILED 	SCP_SOCK_RET_FAILED
#define SUCCESS SCP_SOCK_RET_SUCCESS
#define WRONG 	SCP_SOCK_RET_WRONG
#else
#define 
#define RET_VALUE SCP_RET_VALUE
#define FAILED 	SCP_RET_FAILD
#define SUCCESS SCP_RET_SUCCESS
#define WRONG 	SCP_RET_WRONG
#endif


//客户端或者服务端类型
typedef enum __ScpSockType{
	SCP_CLIENT = 0,
	SCP_SERVER
}ScpSockType;


typedef struct __ScpSockClass{
	int sock;
	struct sockaddr_in addr;
	int8_t cli_or_ser;

	int (*InitSock)(struct __ScpSockClass *, ScpSockType);
	int (*InitAddress)(struct __ScpSockClass*,const char *,uint16_t,int);
	int (*Close)(struct __ScpSockClass*);
/*
 * It's Client Sock Handler.
 * */
	int (*Connect2Server)(struct __ScpSockClass *);
	int (*Read)(struct __ScpSockClass*, char *, int32_t, uint8_t);
	int (*Write)(struct __ScpSockClass*, const char *, int32_t, uint8_t);

/*
 * It's Server Client.
 * */
	int (*Bind)(struct __ScpSockClass*);
	int (*Listen)(struct __ScpSockClass*,int32_t);
	struct __ScpSockClass * (*Accept)(struct __ScpSockClass*); 
}ScpSockClass;
//使用初始化不需要释放
void InitSockClass(ScpSockClass* this);
//使用create申请需要自己释放
ScpSockClass * CreateSockClass(void);
void ReleaseSockClass(ScpSockClass * this);




#endif
