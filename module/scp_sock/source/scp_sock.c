#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <assert.h>
#include "scp_sock.h"


static int initSock(ScpSockClass * this, ScpSockType en_sock_type)
{	
	int ret;
    assert((en_sock_type != SCP_SOCK_CLIENT) && (en_sock_type != SCP_SOCK_SERVER));
	this->sock = socket(AF_INET,SOCK_STREAM,0);
	ret = this->sock;
	if(ret < 0){
		SOCK_DEBUG_PRINT(ret,"Create sock failed.\n");
		return FAILED;
	}
	this->en_cli_or_ser = en_sock_type;
	return SUCCESS;
}


static int initAddress(ScpSockClass* this,const char * ip,int port, int family)
{
	int ret = 0;
	if(this->en_cli_or_ser == SCP_SOCK_CLIENT){
		if((ret = inet_pton(family,ip,(struct sockaddr*)&(this->addr))) < 0){
			SOCK_DEBUG_PRINT(ret, "Inet_pton failed.\n");
			return FAILED;
		}
	}
    else if(this->en_cli_or_ser == SCP_SOCK_SERVER){
		this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	this->addr.sin_family = family;
	this->addr.sin_port = htons(port);
	return SUCCESS;
}

static int connect2Server(ScpSockClass *this)
{
	int ret = 0;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    if(this->en_cli_or_ser < 0){
        SOCK_DEBUG_PRINT(FAILED,"The ScpSockClass is not init\n");
        return FAILED;
    }
	else if(((this->en_cli_or_ser != SCP_SOCK_SERVER))){
		SOCK_DEBUG_PRINT(FAILED, "Only Client sock can use connect.\n");
		return FAILED;
	}
	ret = connect(this->sock,(struct sockaddr*)&(this->addr),sizeof(struct sockaddr_in));
	if(ret != 0){
		SOCK_DEBUG_PRINT(ret, "Connect to server failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int sockRead(ScpSockClass * this, char * recv_buf, int len, int flag)
{
	int recv_len = 0;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
	while(len){
		recv_len = recv(this->sock,recv_buf,len,flag);
		if(recv_len < 0){	
			SOCK_DEBUG_PRINT(recv_len, "Sock Recv buffer failed.\n");
			return FAILED;
		}
		else if(recv_len == 0){
			SOCK_DEBUG_PRINT(WARN,"Sock Recv failed, connect may be invaild.\n");
			return WARN;
		}
		recv_buf += recv_len;
		len -= recv_len;
	}
	return SUCCESS;
}
static int sockWrite(ScpSockClass * this, const char * send_buf, int len, int flag)
{
	int send_len = 0;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    while(len){
        send_len = send(this->sock,send_buf,len,flag);
        if(send_len < 0){
                SOCK_DEBUG_PRINT(send_len,"Sock Send buffer failed.\n");
                return FAILED;
        }
        else if(send_len == 0){
                SOCK_DEBUG_PRINT(WARN,"Sock Send failed, connect may be invaild.\n");
                return WARN;
        }
        send_buf += send_len;
        len -= send_len;
    }
	return SUCCESS;
}
static int sockClose(ScpSockClass * this)
{
	int ret = 0;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
	if((ret = close(this->sock)) != 0){
		SOCK_DEBUG_PRINT(ret,"Close sock failed.\n");
		return FAILED;
	}
	
	return SUCCESS;
}


static int serverBind(ScpSockClass * this)
{
	int ret = 0;
	assert(this->en_cli_or_ser == SCP_SOCK_SERVER);
    if(0 != (ret=bind(this->sock,(struct sockaddr*)&(this->addr),sizeof(struct sockaddr)) ) ){
		SOCK_DEBUG_PRINT(ret,"Bind failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int serverListen(ScpSockClass * this , int count)
{
	int ret = 0;
	assert(this->en_cli_or_ser == SCP_SOCK_SERVER);
	if((ret = listen(this->sock,count)) < 0){
		SOCK_DEBUG_PRINT(ret,"Listen failed.\n");
		return FAILED;
	}
	return SUCCESS;
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
		SOCK_DEBUG_PRINT(ret ,"Accept client failed.\n");
		goto failed;
	}
	cli_this->sock = ret;
	return cli_this;
failed:
    Scp_releaseSockClass(cli_this);
    return NULL;
}

static int setSockReuse(ScpSockClass * this)
{
    int ret = 0;
	int optval = 1;
    socklen_t optlen = sizeof(optval);
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    ret = setsockopt(this->sock,SOL_SOCKET,SO_REUSEADDR,&optval,optlen);
    if(ret < 0){
        SOCK_DEBUG_PRINT(ret,"Set sock reuse failed.\n");
        return ret;
    }
    return SUCCESS;
    
}

static int setTimeout(ScpSockClass * this, int time_ms)
{
	//int ret = 0;
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    struct timeval time = {time_ms,0};
    setsockopt(this->sock,SOL_SOCKET,SO_RCVTIMEO,&time,sizeof(time));
    return SUCCESS;
}
static int getAddr(ScpSockClass * this, char * buf,int buf_len)
{
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    assert(buf && (buf_len >=ADDR_LEN_MIN));
    if(NULL == inet_ntop(AF_INET,&(this->addr.sin_addr),buf,buf_len)){
        SOCK_DEBUG_PRINT(FAILED,"Fun inet_ntop is failed.\n");
        return FAILED;
    }
    return SUCCESS;
}

static char * getAddrOnlyRead(ScpSockClass * this)
{
    assert((this->en_cli_or_ser == SCP_SOCK_CLIENT) || (this->en_cli_or_ser == SCP_SOCK_SERVER));
    return inet_ntoa(this->addr.sin_addr);
}

void Scp_initSockClass(ScpSockClass * this)
{
	memset(this,0,sizeof(ScpSockClass));

    this->en_cli_or_ser = SCP_SOCK_NONE;

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
        SOCK_DEBUG_PRINT(FAILED,"Create ScpSockClass failed.\n");
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



