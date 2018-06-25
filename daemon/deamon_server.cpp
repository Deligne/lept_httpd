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

int 	deamon_proc = 1;

void deamon_inetd(const char *pname, int facility) {
	deamon_proc = 1;
	openlog(pname, LOG_PID, facility);
}

int main(int argc, char const *argv[])
{
	socklen_t len;
	struct sockaddr cliaddr;
	char buff[MAXSIZE];
	time_t ticks;

	deamon_inetd(argv[0], 0);

	len = sizeof(struct sockaddr_storage);
	getpeername(0, &cliaddr, &len);
	// inet_ntop(AF_INET, &cliaddr.sockaddr, buff);
	perror("connection from \n");

	ticks = time(NULL);
	snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	write(0, buff, strlen(buff));

	close(0);
	return 0;
}