#ifndef lept_httpd_H__
#define lept_httpd_H__
#include<stdio.h>  
#include<stdlib.h>
#include<sys/socket.h>  
#include<sys/types.h>  
#include<netinet/in.h>
#include<sys/select.h>
#include<time.h>  
#include<arpa/inet.h>  
#include<unistd.h>  
#include<ctype.h>  
#include<strings.h>  
#include<string.h>  
#include<sys/stat.h>  
#include<pthread.h>  
#include<sys/wait.h>  
#include<stdlib.h>  
#include <cstdlib>
#include<errno.h>

#define MAXSIZE 	1024
#define SERV_PORT	80
#define err_sys(error_message)	do { perror(error_message); exit(-1); } while(0)

int max(int a, int b) {
	return a>b?a:b;
}
#endif

void dg_echo(int sockfd, struct sockaddr* pcliaddr, socklen_t clilen) {
	char	msg[MAXSIZE];
	int 	n;
	socklen_t 	len;
	for(;;) {
		len = clilen;
		n = recvfrom(sockfd, msg, sizeof(msg), 0, pcliaddr, &len);
		sendto(sockfd, msg, n, 0, pcliaddr, len);
	}
}

int main(int argc, char const *argv[])
{
	int listenfd;
	char buf[MAXSIZE];
	int n;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t addrlen;

	addrlen = sizeof(servaddr);
	bzero(&servaddr, addrlen);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	listenfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind(listenfd, (struct sockaddr*) &servaddr, addrlen);
	dg_echo(listenfd, (struct sockaddr*)&cliaddr, addrlen);
	return 0;
}