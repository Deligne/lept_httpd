#ifndef lept_client_H__
#define lept_client_H__
#include<stdio.h>  
#include<stdlib.h>
#include<sys/socket.h>  
#include<sys/types.h>  
#include<netinet/in.h>  
#include<arpa/inet.h>  
#include <sys/select.h>
#include <time.h>
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

int max(int a, int b) {
	return a > b ? a:b;
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
	while(scanf("%s", sendline) > 0) {
		write(sockfd, sendline, strlen(sendline));
		memset(sendline, '\0', sizeof(sendline));
		if(read(sockfd, recvline, MAXSIZE) == 0)
			err_sys("str_cli: server terminated permaturely.");
		fputs(recvline, stdout);
		fputs("\n", stdout);
		memset(recvline, '\0', sizeof(recvline));
	}
}



void str_cli_select(FILE *fp, int sockfd)
{
	int 	maxfdpl;
	fd_set rset;
	char sendline[MAXSIZE], recvline[MAXSIZE];
	FD_ZERO(&rset);
	for(;;) {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdpl = max(fileno(fp), sockfd) + 1;
		select(maxfdpl, &rset, NULL, NULL, NULL);
		if(FD_ISSET(sockfd, &rset)) {
			if(read(sockfd, recvline, sizeof(recvline)) == 0 )
				err_sys("str_cli: server terminated prematurely\n");
			fputs(recvline, stdout);
		}
		str_cli(stdin, sockfd);
		if(FD_ISSET(fileno(stdin), &rset)) {
			if(fgets(sendline, MAXSIZE, stdin) ==NULL) 
				return;
			write(sockfd, sendline, strlen(sendline));
			fputs("\n", stdout);
		}

	}
}



void str_cli_shut(FILE *fp, int sockfd) {
	int stdineof, maxfdpl;
	char buf[MAXSIZE];
	fd_set rset;
	int n;
	FD_ZERO(&rset);
	stdineof = 0;
	for(;;) {
		if(stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdpl = max(fileno(fp), sockfd);
		select(maxfdpl, &rset, NULL, NULL, NULL);
		if(FD_ISSET(sockfd, &rset)) {
			if((n = read(sockfd, buf, sizeof(buf))) == 0) {
				if(stdineof == 1)
					return;
				else 
					err_sys("str_cli_shut: server terminated permaturely.");
			}
			write(fileno(stdout), buf, n);
		}

		if(FD_ISSET(fileno(fp), &rset)) {
			if((n = read(fileno(fp), buf, sizeof(buf)) == 0)) {
				stdineof == 1;
				FD_CLR(fileno(fp), &rset);
				shutdown(sockfd, SHUT_WR);
				continue;
			}
			write(sockfd, buf, n);
		}
		
	}
}




int startup(int argc, char const *argv[]) {
	int sockfd;
	int port = 80;
	struct sockaddr_in server_addr;
	socklen_t len;
	len = sizeof(server_addr);
	if(argc != 2) {
		perror("usage: client_echo <Ipadress>");
		exit(1);
	}
	bzero(&server_addr, len);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if((connect(sockfd, (struct sockaddr *)&server_addr, len)) == -1)
		err_sys("connect");
	return sockfd;
}

#endif