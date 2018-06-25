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
#include<fcntl.h>
#include <netdb.h>
#include <syslog.h>
#define MAXSIZE 1024
#define err_sys(error_message)	do { perror(error_message); exit(-1); } while(0)
#endif


static void sig_alrm(int sig) {
	return;
}

void dg_cli(FILE* fp, int sockfd, const struct sockaddr* pservaddr, socklen_t servlen) {
	int n;
	char sendline[MAXSIZE], recvline[MAXSIZE];

	signal(SIGALRM, sig_alrm);

	while(fgets(sendline, sizeof(sendline), fp) != NULL) {
		sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		alarm(5);

		if((n = recvfrom(sockfd, recvline, sizeof(recvline), 0, NULL, NULL)) < 0) {
			if(errno = EINTR)
				fprintf(stderr, "%s\n", "sock timeout.");
			else 
				fprintf(stderr, "%s\n", "recv error");
		} else {
			alarm(0);
			recvline[n] = 0;
			fputs(recvline, stdout);
		}
	}
}


int read_able(int fd, int sec) {
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(fd, &rset);

	return(select(fd + 1, &rset, NULL, NULL, &tv));
}





















