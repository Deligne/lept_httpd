#include "lept_httpd.cc"

int main(int argc, char const *argv[])
{
	int sockfd;
	sockfd = startup(80);
	struct sockaddr_in client_addr;
	struct sockaddr_in temp_addr;
	char buf[MAXSIZE];
	socklen_t len;
	int connfd;
	pid_t pid;
	len = sizeof(client_addr);
	signal(SIGCHLD, sig_chld);
	sleep(0);
	if((connfd = accept(sockfd, (struct sockaddr *) &client_addr, &len)) < 0)	
		return 0;
	str_cli_shut(stdin, connfd);
	close(connfd);
	exit(0);
}
