/*************************************************************************
	> File Name: test.c
	> Author: chuanpao su
	> Mail: 291657954@qq.com
	> Created Time: Sun 03 Apr 2016 02:29:54 PM CST
 ************************************************************************/

#include<stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "scp_sock/scp_sock.h"

#define server_ip "121.42.163.13"



int main()
{
    int ret;
    char buffer[100]  = {0};
    ScpSockClass st_server;
    Scp_initSockClass(&st_server);
    st_server.initSock(&st_server,SCP_SOCK_SERVER);
    st_server.initAddress(&st_server,server_ip,6666,AF_INET);
    st_server.bind(&st_server);
    st_server.listen(&st_server,10);
    st_server.setTimeout(&st_server,3);
    ScpSockClass *st_client = st_server.accept(&st_server);
    if(!st_client){
        printf("accept timeout : %s,\n",strerror(errno));
    }
    st_client->read(st_client,buffer,99,0);
    printf("11111\n");
    printf("%s connect success\n",inet_ntoa(st_client->addr.sin_addr));



    ScpSockClass * st_test = Scp_createSockClass();

    st_test->initSock(st_test,2);


    return 1;
}
