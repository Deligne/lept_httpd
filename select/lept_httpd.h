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
#define SERV_PORT 80
#define err_sys(error_message)	do { perror(error_message); exit(-1); } while(0)

int max(int a, int b) {
	return a>b?a:b;
}
#endif