#include "lept_client.h"

int main(int argc, char const *argv[])
{
	int sockfd;
	char msg[MAXSIZE];
	int n;
	sockfd = startup(argc, argv);
	// str_cli(stdin, sockfd);
	str_cli_select(stdin, sockfd);
	close(sockfd);
	exit(0);
}
