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

#define MAXSIZE 1024

#define err_sys(error_message)	do { perror(error_message); exit(-1); } while(0)

int max(int a, int b) {
	return a>b?a:b;
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
		if(FD_ISSET(fileno(stdin), &rset)) {
			if(fgets(sendline, MAXSIZE, stdin) ==NULL) 
				return;
			write(sockfd, sendline, strlen(sendline));
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



void Listen(int fd, int backlog);
int startup(u_short );
void echo(int connfd);
int accept_request(int listenfd);
unsigned int get_line(int fd, char *buf, unsigned int size);
void hello_world(int);
#endif