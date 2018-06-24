#ifndef lept_httpd_H__
#define lept_httpd_H__
#include<stdio.h>  
#include<stdlib.h>
#include<sys/socket.h>  
#include<sys/types.h>  
#include <arpa/inet.h>  
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
#include <netdb.h>
#define MAXSIZE 	1024
#define SERV_PORT	80
#define err_sys(error_message)	do { perror(error_message); exit(-1); } while(0)

#define LISTENQ do{ int n; n = getenv("LISTENQ"); return n; } while(0)
int max(int a, int b) {
	return a>b?a:b;
}

struct addrinfo * host_serv(const char *host, const char *serv, int family, int socktype) {
	int 	n;
	struct addrinfo 	hints, *res;
	bzero(&hints, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = family;
	hints.ai_socktype = socktype;

	if((n = getaddrinfo(host, serv, &hints, &res)) != 0)
		return(NULL);
	return(res);
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




#endif