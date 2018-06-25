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

#define MAXFD 	64

int 	deamon_proc;

int deamon_init(const char *pname, int facility) {
	int 	i;
	pid_t 	pid;

	if((pid = fork()) < 0) {
		return -1;
	} else if(pid){
		_exit(0);
	}

	if(setsid() < 0)
		return -1;

	signal(SIGHUP, SIG_IGN);
	if((pid = fork()) < 0)
		return -1;
	else if(pid)
		_exit(0);

	deamon_proc = 1;

	chdir("/");

	for(i = 0; i < MAXFD; i++)
		close(i);

	open("dev/null", O_RDONLY);
	open("dev/null", O_RDWR);
	open("dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);

	return(0);
}



int tcp_connect(const char *host, const char *serv) {
	int 			sockfd, n;
	struct 			addrinfo hints, *res, *ressave;

	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_sys("tep_connect");

	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(sockfd < 0)
			continue;

		if(connect(sockfd, res->ai_addr, res->ai_addrlen) ==0)
			break;
		close(sockfd);
	} while(( res = res->ai_next ) != NULL);

	if(res == NULL) {
		err_sys("tcp_connect error");
	}

	freeaddrinfo(ressave);
	return(sockfd);
}


int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp) {
	int 	n;
	int 	listenfd;
	const int 	on = 1;
	struct addrinfo 	hints, *res, *ressave;
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_sys("getaddrinfo");

	ressave = res;

	do {
		if((listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == 0)
			continue;
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		if(bind(listenfd, res->ai_addr, res->ai_addrlen) ==0 )
			break;
		close(listenfd);

	} while((res = res->ai_next) != NULL);

	listen(listenfd, 5);
	if(addrlenp)
		*addrlenp = res->ai_addrlen;

	freeaddrinfo(ressave);
}



int main(int argc, char const *argv[])
{
	int listenfd, connfd; 
	socklen_t addrlen, len;
	struct sockaddr cliaddr;
	char buff[MAXSIZE];
	time_t ticks;

	deamon_init(argv[0], 0);
	if(argc == 2)
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	else 
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);

	// cliaddr = malloc(addrlen);

	for(;;) {
		len = addrlen;
		connfd = accept(listenfd, &cliaddr, &len);
		// perror("conn fron %s\n", sock_ntop(cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write(connfd, buff, strlen(buff));

		close(connfd);
	}
	return 0;
}