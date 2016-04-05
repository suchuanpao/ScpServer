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
#include "scp_sock/scp_sock.h"

#define server_ip "121.42.163.13"



int main()
{
    ScpSockClass st_client;
    Scp_initSockClass(&st_client);
    st_client.initSock(&st_client,SCP_SOCK_CLIENT);
    st_client.initAddress(&st_client,server_ip,6666,AF_INET);
    st_client.connect2Server(&st_client);
    
    printf("%s connect success\n",inet_ntoa(st_client_sock->addr.sin_addr));
    return 1;
}
