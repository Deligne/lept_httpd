#include<fcntl.h>
#include<stdio.h>
#include "lept_client.h"

int main(int argc, char const *argv[])
{
	int flags;
	int connfd = startup(argc, argv);
	if((flags = fcntl(connfd, F_GETFL, 0)) < 0)
		err_sys("F_GETFL");
	flags |= O_NONBLOCK;
	if(fcntl(connfd, F_SETFL, flags) < 0) 
		err_sys("F_SETFL");
	return 0;
}