#include "lept_client.h"
#include<fcntl.h>

int main(int argc, char const *argv[])
{
	int connfd = startup(argc, argv);
	int flags;
	if((flags = fcntl(connfd, F_GETFL, 0)) < 0)
		err_sys("F_GETFL");
	flags |= O_NONBLOCK;
	if(fcntl(connfd, F_SETFL, flags) < 0) 
		err_sys("F_SETFL");
	str_cli(stdin, connfd);
	return 0;
}