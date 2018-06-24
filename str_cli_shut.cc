#include "lept_client.h"

int main(int argc, char const *argv[])
{
	int sockfd = 0;
	sockfd = startup(argc, argv);
	str_cli_shut(stdin, sockfd);
	return 0;
}