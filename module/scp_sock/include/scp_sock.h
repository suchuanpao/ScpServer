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
//#include "ScpServer/scp_msg.h"
//如果未定义日志记录错误的宏，则直接输出
#ifndef PRINT
#define SCP_DEBUG_PRINT(ret,err_str) printf("%s,%s,%d:%sreturn value %d\n",__FILE__,__func__,__LINE__,err_str,ret)
#endif


//模块内部使用的返回值宏
enum SCP_SOCK_RET_VALUE{
	SCP_SOCK_RET_FAILED = -1,
	SCP_SOCK_RET_SUCCESS,
	SCP_SOCK_RET_WARN
};

//返回值:全局宏定义若定义，则使用SCP_RET_VALUE作为统一返回宏
#ifndef GLOBAL_RET_CORE
#define RET_VALUE SCP_SOCK_RET_VALUE
#define FAILED 	SCP_SOCK_RET_FAILED
#define SUCCESS SCP_SOCK_RET_SUCCESS
#define WARN 	SCP_SOCK_RET_WARN
#else
#define 
#define RET_VALUE SCP_RET_VALUE
#define FAILED 	SCP_RET_FAILD
#define SUCCESS SCP_RET_SUCCESS
#define WARN 	SCP_RET_WARN
#endif


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
	int (*initSock)(struct __ScpSockClass *, ScpSockType);
	int (*initAddress)(struct __ScpSockClass*,const char *,uint16_t,int);
	int (*close)(struct __ScpSockClass*);
//套接字的操作
	int (*connect2Server)(struct __ScpSockClass *);
	int (*read)(struct __ScpSockClass*, char *, int32_t, uint8_t);
	int (*write)(struct __ScpSockClass*, const char *, int32_t, uint8_t);


//  It's Server Client.
	int (*bind)(struct __ScpSockClass*);
	int (*listen)(struct __ScpSockClass*,int32_t);
	struct __ScpSockClass * (*accept)(struct __ScpSockClass*); 
}ScpSockClass;



//create or init ScpSockClass
//使用初始化不需要释放
void Scp_initSockClass(ScpSockClass* this);
//使用create申请需要自己释放
ScpSockClass * Scp_createSockClass(void);
void Scp_releaseSockClass(ScpSockClass * this);




#endif
