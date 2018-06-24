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

void dg_cli(FILE *fp, int sockfd, struct sockaddr *servaddr, socklen_t len) {
	char msg[MAXSIZE], recvline[MAXSIZE+1];
	int n;
	while(fgets( msg, sizeof(msg), fp) != NULL) {
		sendto(sockfd, msg, strlen(msg), 0, servaddr, len);
		if((n = recvfrom(sockfd, recvline, sizeof(recvline), 0, NULL, NULL)) <= 0)
			err_sys("recv");
		recvline[n] = 0;
		fputs(recvline, stdout);
	}
}

void dg_cli_conn(FILE *fp, int sockfd, struct sockaddr *servaddr, socklen_t len) {
	int 	n;
	char sendline[MAXSIZE], recvline[MAXSIZE];

	connect(sockfd, (struct sockaddr *) servaddr, len);

	while(fgets(sendline, MAXSIZE, fp) != NULL) {
		write(sockfd, sendline, strlen(sendline));
		n = read(sockfd, recvline, sizeof(recvline));
		recvline[n] == 0;
		fputs(recvline, stdout);
	}
}


int main(int argc, char const *argv[])
{
	struct sockaddr_in servaddr;
	socklen_t len = sizeof(servaddr);
	int n;
	char buf[MAXSIZE];
	int sockfd;
	bzero(&servaddr, len);
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	dg_cli_conn(stdin, sockfd, (struct sockaddr*) &servaddr, len);
	return 0;
}