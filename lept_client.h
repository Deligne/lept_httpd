#ifndef lept_client_H__
#define lept_client_H__
#include<stdio.h>  
#include<stdlib.h>
#include<sys/socket.h>  
#include<sys/types.h>  
#include<netinet/in.h>  
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
#define MAXSIZE 1024
#define err_sys(error_message)	do { perror(error_message); exit(-1); } while(0)

void chat(int sockfd) {
	char msg[MAXSIZE];
	int n;
	printf("chat\n");
	while(1) {
		fgets(msg, MAXSIZE, stdin);
		write(sockfd, msg, strlen(msg));
		printf("send: %s\n", msg);
		if((n = read(sockfd, msg, sizeof(msg))) > 0) {
			msg[n] = '\0';
			printf("recv: %s\n", msg);
		}
	}
	exit(0);
}

void str_add(int connfd) {
	char buf[MAXSIZE];
	char arg[MAXSIZE];
	fgets(arg, MAXSIZE, stdin);
	write(connfd, arg, strlen(arg));
	int n = read(connfd, buf, sizeof(buf));
	printf("%s\n", arg);
	buf[n] = '\0';
	printf("%s\n", buf);
}

void str_echo(int sockfd) {
	ssize_t n;
	char buf[MAXSIZE];
again:
	while ((n = read(sockfd, buf, MAXSIZE)) > 0){
		buf[n] = '\0';
		printf("recv: %s.\n", buf);
		write(sockfd, buf, n);
	}
	if(n<0 && errno == EINTR) {
		goto again;
	} else if (n < 0) {
		err_sys("str_echo:read error");
	}
}

void str_cli(FILE *fp, int sockfd) {
	char sendline[MAXSIZE], recvline[MAXSIZE];
	int n;
	while(fgets(sendline, MAXSIZE, fp)!= NULL) {
		write(sockfd, sendline, strlen(sendline));
		bzero(sendline, sizeof(sendline));
		if(read(sockfd, recvline, MAXSIZE) == 0)
			err_sys("str_cli: server terminated permaturely.");
		fputs(recvline, stdout);
	}
}

#endif