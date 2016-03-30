#include "scp_sock.h"



static int InitSockClass(ScpSockClass * sock_class, ScpSockType sock_type)
{	
	int ret;
	sock_class->cli_or_ser = sock_type;
	sock_class->sock = socket(AF_INET,SOCK_STREAM,0);
	ret = sock_class->sock;
	if(ret < 0){
		SCP_PERROR(ret, "Create sock failed.\n");
		return FAILED;
	}
	return SUCCESS;
}


static int InitAddress(ScpSockClass* sock_class,const char * ser_ip,uint16_t port, int family)
{
	int ret = 0;
	sock_class->addr.sin_family = family;
	sock_class->addr.sin_port = htons(port);
	if(sock_class->cli_or_ser == SCP_CLIENT){
		if((ret = inet_pton(family,ser_ip,(struct sockaddr*)&(sock_class->addr))) < 0){
			SCP_PERROR(ret, "Inet_pton failed.\n");
			return FAILED;
		}
	}else{
		sock_class->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	return SUCCESS;
}

static int Connect2Server(ScpSockClass *sock_class)
{
	int ret = 0;
	if(sock_class->cli_or_ser != SCP_CLIENT){
		SCP_PERROR(ret, "Server sock can not use as client sock.\n");
		return FAILED;
	}
	if((ret = connect(sock_class->sock,(struct sockaddr*)&(sock_class->addr),sizeof(struct sockaddr_in)))){
		SCP_PERROR(ret , "Connect to server failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int SockRead(ScpSockClass * sock_class, char * recv_buf, int32_t len, uint8_t flag){
	int recv_len = 0;
	while(len){
		recv_len = read(sock_class->sock,recv_buf,len,flag);
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
static int SockWrite(ScpSockClass * sock_class, const char * send_buf, int32_t len, uint8_t flag)
{
	int send_len = 0;
        while(len){
                send_len = read(sock_class->sock,send_buf,len,flag);
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
static int Close(ScpSockClass * sock_class)
{
	int ret = 0;
	if((ret = close(sock_class->sock)) != 0){
		SCP_PERROR(ret,"Close sock failed.\n");
		return FAILED;
	}
	return SUCCESS;
}


static int Bind(ScpSockClass * sock_class)
{
	int ret = 0;
	if(sock_class->cli_or_ser != 0){
		SCP_PERROR(0,"Client sock can not bind adderss.\n");
	}
	if(0 != (ret=bind(sock_class->sock,(struct sockaddr*)&(sock_class->addr),sizeof(struct sockaddr)) ) ){
		SCP_PERROR(ret,"Bind failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int Listen(ScpSockClass * sock_class , int32_t count)
{
	int ret = 0;
	if(sock_class->cli_or_ser != 0){
                SCP_PERROR(0,"Client sock can not listen to connect.\n");
		return FAILED;
        }
	if((ret = listen(sock_class->sock,count)) < 0){
		SCP_PERROR(ret,"Listen failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static ScpSockClass * Accept(ScpSockClass * sock_class)
{	
	int ret = 0;
	ScpSockClass * cli_sock_class= NULL;
	cli_sock_class = CreateSockClass();

	socklen_t cli_len;
	
        if(sock_class->cli_or_ser != 0){
                SCP_PERROR(0,"Client sock can not Accept.\n");
                return FAILED;
        }	
	if((ret = accept(sock_class->sock,(struct sockaddr*)&(cli_sock_class->addr),&cli_len)) < 0){
		SCP_PERROR(ret ,"Accept client failed.\n");
		return FAILED;
	}
	cli_sock_class->sock = ret;
	return cli_sock_class;
}



ScpSockClass * CreateSockClass(void)
{
	ScpSockClass * sock_class = malloc(sizeof(ScpSockClass));
	if(sock_class == NULL){
		SCP_PERROR(0,"ScpSockClass malloc failed.\n");
		return NULL;
	}
	memset(sock_class,0,sizeof(ScpSockClass));

	sock_class->InitSockClass = InitSockClass;
	sock_class->InitAddress = InitAddress;
	sock_class->Close = Close;

	sock_class->Connect2Server = Connect2Server;
	sock_class->Read = SockRead;
	sock_class->Write = SockWrite;

	sock_class->Bind = Bind;
	sock_class->Listen = Listen;
	sock_class->Accept = Accept;
	return sock_class;
}

int ReleaseSockClass(ScpSockClass * sock_class)
{
	memset(sock_class,0,sizeof(ScpSockClass));
	free(sock_class);	
}






	
