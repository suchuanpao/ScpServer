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
#include "scp_sock.h"

#define server_ip "121.42.163.13"



int main()
{
    ScpSockClass st_server;
    Scp_initSockClass(&st_server);
    st_server.initSock(&st_server,SCP_SOCK_SERVER);
    st_server.initAddress(&st_server,server_ip,6666,AF_INET);
    st_server.bind(&st_server);
    st_server.listen(&st_server,10);
    ScpSockClass *st_client_sock = st_server.accept(&st_server);
    printf("%s connect success\n",inet_ntoa(st_client_sock->addr.sin_addr));
    return 1;
}
