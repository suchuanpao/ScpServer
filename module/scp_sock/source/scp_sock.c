#include <string.h>
#include <stdlib.h>
#include "scp_sock.h"


static int initSock(ScpSockClass * this, ScpSockType en_sock_type)
{	
	int ret;
	this->en_cli_or_ser = en_sock_type;
	this->sock = socket(AF_INET,SOCK_STREAM,0);
	ret = this->sock;
	if(ret < 0){
		SCP_DEBUG_PRINT(ret, "Create sock failed.\n");
		return FAILED;
	}
	return SUCCESS;
}


static int initAddress(ScpSockClass* this,const char * ser_ip,uint16_t port, int family)
{
	int ret = 0;
    if(this->en_cli_or_ser < 0){
        SCP_DEBUG_PRINT(FAILED,"The ScpSockClass is not init\n");
        return FAILED;
    }
	if(this->en_cli_or_ser == SCP_SOCK_CLIENT){
		if((ret = inet_pton(family,ser_ip,(struct sockaddr*)&(this->addr))) < 0){
			SCP_DEBUG_PRINT(ret, "Inet_pton failed.\n");
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
    if(this->en_cli_or_ser < 0){
        SCP_DEBUG_PRINT(FAILED,"The ScpSockClass is not init\n");
        return FAILED;
    }
	else if(this->en_cli_or_ser != SCP_SOCK_CLIENT){
		SCP_DEBUG_PRINT(FAILED, "Only Client sock can use connect.\n");
		return FAILED;
	}
	else((ret = connect(this->sock,(struct sockaddr*)&(this->addr),sizeof(struct sockaddr_in)))){
		SCP_DEBUG_PRINT(ret, "Connect to server failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int sockRead(ScpSockClass * this, char * recv_buf, int32_t len, uint8_t flag)
{
	int recv_len = 0;
    if(this->en_cli_or_ser < 0){
        SCP_DEBUG_PRINT(FAILED,"The ScpSockClass is not init\n");
        return FAILED;
    }
	while(len){
		recv_len = recv(this->sock,recv_buf,len,flag);
		if(recv_len < 0){	
			SCP_DEBUG_PRINT(recv_len, "Sock Recv buffer failed.\n");
			return FAILED;
		}
		else if(recv_len == 0){
			SCP_DEBUG_PRINT(WARN,"Sock Recv failed, connect may be invaild.\n");
			return WARN;
		}
		recv_buf += recv_len;
		len -= recv_len;
	}
	return SUCCESS;
}
static int sockWrite(ScpSockClass * this, const char * send_buf, int32_t len, uint8_t flag)
{
	int send_len = 0;
    if(this->en_cli_or_ser < 0){
        SCP_DEBUG_PRINT(FAILED,"The ScpSockClass is not init\n");
        return FAILED;
    }
    while(len){
        send_len = send(this->sock,send_buf,len,flag);
        if(send_len < 0){
                SCP_DEBUG_PRINT(send_len,"Sock Send buffer failed.\n");
                return FAILED;
        }
        else if(send_len == 0){
                SCP_DEBUG_PRINT(WARN,"Sock Send failed, connect may be invaild.\n");
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
    if(this->en_cli_or_ser < 0){
        SCP_DEBUG_PRINT(FAILED,"The ScpSockClass is not init\n");
        return FAILED;
    }
	if((ret = close(this->sock)) != 0){
		SCP_DEBUG_PRINT(ret,"Close sock failed.\n");
		return FAILED;
	}
	
	return SUCCESS;
}


static int serverBind(ScpSockClass * this)
{
	int ret = 0;
    if(this->en_cli_or_ser < 0){
        SCP_DEBUG_PRINT(FAILED,"The ScpSockClass is not init\n");
        return FAILED;
    }
	else if(this->en_cli_or_ser != SCP_SOCK_SERVER){
		SCP_DEBUG_PRINT(FAILED,"Only Server sock can use bind handler.\n");
	}
	else if(0 != (ret=bind(this->sock,(struct sockaddr*)&(this->addr),sizeof(struct sockaddr)) ) ){
		SCP_DEBUG_PRINT(ret,"Bind failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int serverListen(ScpSockClass * this , int32_t count)
{
	int ret = 0;
    if(this->en_cli_or_ser < 0){
        SCP_DEBUG_PRINT(FAILED,"The ScpSockClass is not init\n");
        return FAILED;
    }
	else if(this->en_cli_or_ser != SCP_SOCK_SERVER){
                SCP_DEBUG_PRINT(0,"Only server sock can use listen handler.\n");
		return FAILED;
        }
	if((ret = listen(this->sock,count)) < 0){
		SCP_DEBUG_PRINT(ret,"Listen failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static ScpSockClass * serverAccept(ScpSockClass * this)
{	
	int ret = 0;
    if(this->en_cli_or_ser < 0){
        SCP_DEBUG_PRINT(FAILED,"The ScpSockClass is not init\n");
        return NULL;
    }
	ScpSockClass * cli_this= NULL;

	cli_this = Scp_createSockClass();
	socklen_t cli_len = sizeof(struct sockaddr_in);

    if(this->en_cli_or_ser != 0){
         SCP_DEBUG_PRINT(FAILED,"Only server sock can use accept handler.\n");
         return NULL;
    }	
	if((ret = accept(this->sock,(struct sockaddr*)&(cli_this->addr),&cli_len)) < 0){
		SCP_DEBUG_PRINT(ret ,"Accept client failed.\n");
		return NULL;
	}
	cli_this->sock = ret;
	return cli_this;
}

void Scp_initSockClass(ScpSockClass * this)
{
	memset(this,0,sizeof(ScpSockClass));

    this->en_cli_or_ser = -1;

	this->initSock = initSock;
	this->initAddress = initAddress;
	this->close = sockClose;

	this->connect2Server = connect2Server;
	this->read = sockRead;
	this->write = sockWrite;

	this->bind = serverBind;
	this->listen = serverListen;
	this->accept = serverAccept;
}


ScpSockClass * Scp_createSockClass(void)
{
	ScpSockClass * this = malloc(sizeof(ScpSockClass));
	if(this == NULL){
		SCP_DEBUG_PRINT(FAILED,"ScpSockClass malloc failed.\n");
		return NULL;
	}
	memset(this,0,sizeof(ScpSockClass));

    this->en_cli_or_ser = -1;

	this->initSock = initSock;
	this->initAddress = initAddress;
	this->close = sockClose;

	this->connect2Server = connect2Server;
	this->read = sockRead;
	this->write = sockWrite;

	this->bind = serverBind;
	this->listen = serverListen;
	this->accept = serverAccept;

	return this;
}

void Scp_releaseSockClass(ScpSockClass * this)
{
    if(!this){
        SCP_DEBUG_PRINT(WARN,"This sock class is NULL already.\n")
    }
	memset(this,0,sizeof(ScpSockClass));
	free(this);	
}



