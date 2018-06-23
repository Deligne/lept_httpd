#include "lept_client.h"

int main(int argc, char const *argv[])
{
	struct sockaddr_in addr;
	int sockfd[5];
	if(argc != 2) {
		err_sys("usage: filename  <IPaddress>");
	}
	for (int i = 0; i < 5; ++i)	{
		sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(0);
		inet_pton(AF_INET, argv[1], &addr.sin_addr);
		connect(sockfd[i], (struct sockaddr *) &addr, sizeof(addr));
		str_cli(stdin, sockfd[i]);
	}
	str_cli(stdin, sockfd[0]);
	exit(0);
}