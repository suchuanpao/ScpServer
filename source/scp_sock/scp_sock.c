#include "scp_sock.h"



static int InitSockClass(ScpSockClass * sock_class, ScpSockType sock_type)
{
	scp_class->cli_or_ser = sock_type;
	scp_class->sock = socket(AF_INET,SOCK_STREAM,0);
	if(scp_sock->sock < 0){
		SCP_PERROR("Create sock failed.\n");
		return FAILED;
	}
	return SUCCESS;
}


static int InitAddress(ScpSockClass* sock_class,const char * ser_ip,uint16_t port, int familly)
{
	sock_class->addr.sin_familly = familly
	sock_class->addr.sin_port = htons(port);
	if(sock_class->cli_or_ser == SCP_CLIENT){
		if(inet_pton(familly,ser_ip,(struct sockaddr*)&(sock_class->addr)) < 0){
			SCP_PERROR("Inet_pton failed.\n");
			return FAILED;
		}
	}else{
		sock_class->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	return SUCCESS;
}

static int Connect2Server(ScpSockClass *sock_class)
{
	if(sock_class->cli_or_ser != SCP_CLIENT){
		SCP_PERROR("Server sock can not use as client sock.\n");
		return FAILED;
	}
	if(connect(sock_class->sock,(struct sockaddr*)&(sock_class->addr),sizeof(struct sockaddr_in))){
		SCP_PERROR("Connect to server failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int SockRead(ScpSockClass * sock_class, char * recv_buf, int32_t len, uint8_t flag){
	int recv_len = 0;
	while(len){
		recv_len = read(sock_class->sock,recv_buf,len,flasg);
		if(recv_len < 0){
			SCP_PERROR("Sock Recv buffer failed.\n");
			return FAILED;
		}
		if(recv_len == 0){
			SCP_PERROR("Sock Recv failed, connect may be invaild.\n");
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
                send_len = read(sock_class->sock,send_buf,len,flasg);
                if(send_len < 0){
                        SCP_PERROR("Sock Send buffer failed.\n");
                        return FAILED;
                }
                if(send_len == 0){
                        SCP_PERROR("Sock Send failed, connect may be invaild.\n");
                        return WRONG;
                }
		send_buf += send_len;
		len -= send_len;
        }
	return SUCCESS;
}
static int Close(ScpSockClass * sock_class)
{
	if(close(sock_class->sock) != 0){
		SCP_PERROR("Close sock failed.\n");
		return FAILED;
	}
	return SUCCESS;
}


static int Bind(ScpSockClass * sock_class)
{
	if(sock_class->cli_or_ser != 0){
		SCP_PERROR("Client sock can not bind adderss.\n");
	}
	if(0 != bind(sock_class->sock,(struct sockaddr*)&(sock_class->addr),sizeof(struct sockaddr))){
		SCP_PERROR("Bind failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static int Listen(ScpSockClass * sock_class , int32_t count)
{
	if(sock_class->cli_or_ser != 0){
                SCP_PERROR("Client sock can not listen to connect.\n");
		return FAILED;
        }
	if(listen(sock_class->sock,count) < 0){
		SCP_PERROR("Listen failed.\n");
		return FAILED;
	}
	return SUCCESS;
}

static ScpSockClass * Accept(ScpSockClass *)
{	
	int ret = 0;
	int cli_sock;
	struct sockaddr_in cli_addr;
	socklen_t cli_len;
	
        if(sock_class->cli_or_ser != 0){
                SCP_PERROR("Client sock can not Accept.\n");
                return FAILED;
        }	
	ret = accept(sock_addr->sock,(struct sockaddr*)&cli_addr,&cli_len);
	if(ret < 0){
		SCP_PERROR("Accept client failed.\n");
		return FAILED;
	}
	return SUCCESS;
}



struct ScpSockClass * CreateSockClass(void)
{
	struct ScpSockClass * sock_class = malloc(sizeof(struct ScpSockClass));
	if(sock_class == NULL){
		SCP_PERROR("ScpSockClass malloc failed.\n");
		return NULL;
	}
	sock_class->InitSockClass = InitSockClass;
	sock_class->InitAddress = InitAddress;
	sock_class->Connect2Server = Connect2Server;
	sock_class->Close = Close;
	sock_class->Read = SockRead;
	sock_class->Write = SockWrite;
	
}
int ReleaseSockClass(ScpSockClass * sock_class);






	
