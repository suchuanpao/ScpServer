#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
//#include "Scp_errString.h"
#include "ScpSock.h"




#ifdef SCP_SOCK_ERRSTR
#define SOCK_ERR_STRING((ret),(str)) \
        this->err_no = ret; \
        sprintf(this->err_string,"%s,%s,%d", \
        __FILE__,__func__,__LINE__); \
        strncpy(this->err_reason,(str),sizeof(this->err_reason)-1)

static char * err_str(ScpSockClass * this)
{
    if(this->err_no == SCP_SOCK_FAILED){

        return this->err_reason;
    }
    else if(this->err_no == SCP_SOCK_MY_FAILED){
        strncat(this->err_string,this->err_reason,sizeof(this->err_string));
        return this->err_string;
    }
    return "";
    
}
#else

#define SOCK_ERR_STRING(ret,str)         

#endif



//***************************************公用操作操作***********************************
static int initSock(ScpSockClass * this, ScpSockType en_sock_type)
{	
	int ret;
    assert((en_sock_type != SCP_SOCK_CLIENT) && (en_sock_type != SCP_SOCK_SERVER));
	this->sock = socket(AF_INET,SOCK_STREAM,0);
	ret = this->sock;
	if(ret < 0){
		SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(errno));
		return SCP_SOCK_FAILED;
	}
	this->en_cli_or_ser = en_sock_type;
	return SCP_SOCK_SUCCESS;
}


static int initAddress(ScpSockClass* this,const char * ip,int port, int family)
{
	int ret = 0;
	if(this->en_cli_or_ser == SCP_SOCK_CLIENT){
		if((ret = inet_pton(family,ip,(struct sockaddr*)&(this->addr))) < 0){
			SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(errno));
			return SCP_SOCK_FAILED;
		}
	}
    else if(this->en_cli_or_ser == SCP_SOCK_SERVER){
		this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	this->addr.sin_family = family;
	this->addr.sin_port = htons(port);
	return SCP_SOCK_SUCCESS;
}

static int sockRead(ScpSockClass * this, char * recv_buf, int len, int flag)
{
	int recv_len = 0;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
	while(len){
		recv_len = recv(this->sock,recv_buf,len,flag);
		if(recv_len <= 0){	
			SOCK_ERR_STRING(SCP_SOCK_FAILED, strerrno(errno));
			return SCP_SOCK_FAILED;
		}
		recv_buf += recv_len;
		len -= recv_len;
	}
	return SCP_SOCK_SUCCESS;
}
static int sockWrite(ScpSockClass * this, const char * send_buf, int len, int flag)
{
	int send_len = 0;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    while(len){
        send_len = send(this->sock,send_buf,len,flag);
        if(send_len <= 0){
                SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(errno));
                return SCP_SOCK_FAILED;
        }
        send_buf += send_len;
        len -= send_len;
    }
	return SCP_SOCK_SUCCESS;
}
static int sockClose(ScpSockClass * this)
{
	int ret = 0;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
	if((ret = close(this->sock)) != 0){
		SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(errno));
		return SCP_SOCK_FAILED;
	}
	
	return SCP_SOCK_SUCCESS;
}
//***************************************客户端操作***********************************
static int connect2Server(ScpSockClass *this)
{
	int ret = 0;
    assert(this->en_cli_or_ser == SCP_SOCK_CLIENT);
	ret = connect(this->sock,(struct sockaddr*)&(this->addr),sizeof(struct sockaddr_in));
	if(ret != 0){
		SOCK_ERR_STRING(SCP_SOCK_FAILED, strerrno(errno));
		return SCP_SOCK_FAILED;
	}
	return SCP_SOCK_SUCCESS;
}


//***************************************服务器套接字操作***********************************
static int serverBind(ScpSockClass * this)
{
	int ret = 0;
	assert(this->en_cli_or_ser == SCP_SOCK_SERVER);
    if(0 != (ret=bind(this->sock,(struct sockaddr*)&(this->addr),sizeof(struct sockaddr)) ) ){
		SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(errno));
		return SCP_SOCK_FAILED;
	}
	return SCP_SOCK_SUCCESS;
}

static int serverListen(ScpSockClass * this , int count)
{
	int ret = 0;
	assert(this->en_cli_or_ser == SCP_SOCK_SERVER);
	if((ret = listen(this->sock,count)) < 0){
		SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(errno));
		return SCP_SOCK_FAILED;
	}
	return SCP_SOCK_SUCCESS;
}

static ScpSockClass * serverAccept(ScpSockClass * this)
{	
	int ret = 0;
    assert(this->en_cli_or_ser == SCP_SOCK_SERVER);

	ScpSockClass * cli_this= NULL;
	cli_this = Scp_createSockClass();
    if(!cli_this){
        return NULL;
    }
	socklen_t cli_len = sizeof(struct sockaddr_in);

	if((ret = accept(this->sock,(struct sockaddr*)&(cli_this->addr),&cli_len)) < 0){
		SOCK_ERR_STRING(SCP_SOCK_FAILED ,strerrno(errno));
		goto SCP_SOCK_FAILED;
	}
	cli_this->sock = ret;
	return cli_this;
SCP_SOCK_FAILED:
    Scp_releaseSockClass(cli_this);
    return NULL;
}

//***************************************套接字操作***********************************
static int setSockReuse(ScpSockClass * this)
{
    int ret = 0;
	int optval = 1;
    socklen_t optlen = sizeof(optval);
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    ret = setsockopt(this->sock,SOL_SOCKET,SO_REUSEADDR,&optval,optlen);
    if(ret < 0){
        SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(errno));
        return SCP_SOCK_FAILED;
    }
    return SCP_SOCK_SUCCESS;
    
}

static int setTimeout(ScpSockClass * this, int  time_s,int time_ms)
{
    int ret = 0;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    struct timeval time = {time_s,time_ms};
    ret = setsockopt(this->sock,SOL_SOCKET,SO_RCVTIMEO,&time,sizeof(time));
    if(ret < 0){
        SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(errno));
    }
    return SCP_SOCK_SUCCESS;
}

static int getAddr(ScpSockClass * this, char * buf,int buf_len)
{
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    assert(buf && (buf_len >=ADDR_LEN_MIN));
    if(NULL == inet_ntop(AF_INET,&(this->addr.sin_addr),buf,buf_len)){
        SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(errno));
        return SCP_SOCK_FAILED;
    }
    return SCP_SOCK_SUCCESS;
}

static char * getAddrOnlyRead(ScpSockClass * this)
{
    char *str = NULL;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    str = inet_ntoa(this->addr.sin_addr);
    if(!str){
        SOCK_ERR_STRING(SCP_SOCK_FAILED,strerrno(err_no));
        return NULL;
    }
    return str;
}

//***************************************线程锁操作***********************************
static int useByThreadMutex(ScpSockClass * this, pthread_t * tid)
{
#ifdef SCP_SOCK_ERRSTR
    
#endif
}

void Scp_initSockClass(ScpSockClass * this)
{
	memset(this,0,sizeof(ScpSockClass));

    this->en_cli_or_ser = SCP_SOCK_FAILED;
#ifdef SCP_SOCK_ERRSTR
    memset(&st_err_head,0,sizeof(ErrorMsg));
    this->err_str = err_str;
#endif
	this->initSock = initSock;
	this->initAddress = initAddress;
	this->close = sockClose;

	this->connect2Server = connect2Server;
	this->read = sockRead;
	this->write = sockWrite;

	this->bind = serverBind;
	this->listen = serverListen;
	this->accept = serverAccept;

    this->getAddrOnlyRead = getAddrOnlyRead;
    this->getAddr = getAddr;
    this->setTimeout = setTimeout;
    this->setSockReuse = setSockReuse;
}


ScpSockClass * Scp_createSockClass(void)
{
	ScpSockClass * this = malloc(sizeof(ScpSockClass));
    if(this != NULL){
        SOCK_ERR_STRING(SCP_SOCK_FAILED,"Create ScpSockClass SCP_SOCK_FAILED.\n");
    }
    Scp_initSockClass(this);
	return this;
}

void Scp_releaseSockClass(ScpSockClass * this)
{
    assert(this);
	memset(this,0,sizeof(ScpSockClass));
	free(this);	
}



