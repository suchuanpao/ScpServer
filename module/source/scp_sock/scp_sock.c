#include <string.h>
#include <stdlib.h>
#include "ScpServer/scp_sock.h"


static int InitSock(ScpSockClass * this, ScpSockType sock_type)
{	
	int ret;
	this->cli_or_ser = sock_type;
	this->sock = socket(AF_INET,SOCK_STREAM,0);
	ret = this->sock;
	if(ret < 0){
		SCP_PERROR(ret, "Create sock failed.\n");
		return FAILED;
	}
	return SUCCESS;
}


static int InitAddress(ScpSockClass* this,const char * ser_ip,uint16_t port, int family)
{
	int ret = 0;
	this->addr.sin_family = family;
	this->addr.sin_port = htons(port);
	if(this->cli_or_ser == SCP_CLIENT){
		if((ret = inet_pton(family,ser_ip,(struct sockaddr*)&(this->addr))) < 0){
			SCP_PERROR(ret, "Inet_pton failed.\n");
			return FAILED;
		}
	}else{
		this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	return SUCCESS;
}

static int Connect2Server(ScpSockClass *this)
{
	int ret = 0;
	if(this->cli_or_ser != SCP_CLIENT){
		SCP_PERROR(ret, "Server sock can not use as client sock.\n");
		return FAILED;
	}
	if((ret = connect(this->sock,(struct sockaddr*)&(this->addr),sizeof(struct sockaddr_in)))){
		SCP_PERROR(ret , "Connect to server failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int SockRead(ScpSockClass * this, char * recv_buf, int32_t len, uint8_t flag){
	int recv_len = 0;
	while(len){
		recv_len = recv(this->sock,recv_buf,len,flag);
		if(recv_len < 0){	
			SCP_PERROR(recv_len, "Sock Recv buffer failed.\n");
			return FAILED;
		}
		if(recv_len == 0){
			SCP_PERROR(0,"Sock Recv failed, connect may be invaild.\n");
			return WRONG;
		}
		recv_buf += recv_len;
		len -= recv_len;
	}
	return SUCCESS;
}
static int SockWrite(ScpSockClass * this, const char * send_buf, int32_t len, uint8_t flag)
{
	int send_len = 0;
        while(len){
                send_len = send(this->sock,send_buf,len,flag);
                if(send_len < 0){
                        SCP_PERROR(send_len,"Sock Send buffer failed.\n");
                        return FAILED;
                }
                if(send_len == 0){
                        SCP_PERROR(0,"Sock Send failed, connect may be invaild.\n");
                        return WRONG;
                }
		send_buf += send_len;
		len -= send_len;
        }
	return SUCCESS;
}
static int Close(ScpSockClass * this)
{
	int ret = 0;
	if((ret = close(this->sock)) != 0){
		SCP_PERROR(ret,"Close sock failed.\n");
		return FAILED;
	}
	
	return SUCCESS;
}


static int Bind(ScpSockClass * this)
{
	int ret = 0;
	if(this->cli_or_ser != 0){
		SCP_PERROR(0,"Client sock can not bind adderss.\n");
	}
	if(0 != (ret=bind(this->sock,(struct sockaddr*)&(this->addr),sizeof(struct sockaddr)) ) ){
		SCP_PERROR(ret,"Bind failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int Listen(ScpSockClass * this , int32_t count)
{
	int ret = 0;
	if(this->cli_or_ser != 0){
                SCP_PERROR(0,"Client sock can not listen to connect.\n");
		return FAILED;
        }
	if((ret = listen(this->sock,count)) < 0){
		SCP_PERROR(ret,"Listen failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static ScpSockClass * Accept(ScpSockClass * this)
{	
	int ret = 0;
	ScpSockClass * cli_this= NULL;
	cli_this = CreateSockClass();

	socklen_t cli_len;
	
        if(this->cli_or_ser != 0){
                SCP_PERROR(0,"Client sock can not Accept.\n");
                return FAILED;
        }	
	if((ret = accept(this->sock,(struct sockaddr*)&(cli_this->addr),&cli_len)) < 0){
		SCP_PERROR(ret ,"Accept client failed.\n");
		return FAILED;
	}
	cli_this->sock = ret;
	return cli_this;
}

void InitSockClass(ScpSockClass * this)
{
	memset(this,0,sizeof(ScpSockClass));

	this->InitSock = InitSock;
	this->InitAddress = InitAddress;
	this->Close = Close;

	this->Connect2Server = Connect2Server;
	this->Read = SockRead;
	this->Write = SockWrite;

	this->Bind = Bind;
	this->Listen = Listen;
	this->Accept = Accept;
	return this;
}


ScpSockClass * CreateSockClass(void)
{
	ScpSockClass * this = malloc(sizeof(ScpSockClass));
	if(this == NULL){
		SCP_PERROR(0,"ScpSockClass malloc failed.\n");
		return NULL;
	}
	memset(this,0,sizeof(ScpSockClass));

	this->InitSock = InitSock;
	this->InitAddress = InitAddress;
	this->Close = Close;

	this->Connect2Server = Connect2Server;
	this->Read = SockRead;
	this->Write = SockWrite;

	this->Bind = Bind;
	this->Listen = Listen;
	this->Accept = Accept;
	return this;
}

void ReleaseSockClass(ScpSockClass * this)
{
	memset(this,0,sizeof(ScpSockClass));
	free(this);	
}



