#include "lept_client.h"

int startup(int argc, char const *argv[]) {
	int sockfd;
	int port = 80;
	struct sockaddr_in server_addr;
	socklen_t len;
	len = sizeof(server_addr);
	if(argc != 2) {
		perror("usage: client_echo <Ipadress>");
		exit(1);
	}
	bzero(&server_addr, len);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if((connect(sockfd, (struct sockaddr *)&server_addr, len)) == -1)
		err_sys("connect");
	printf("connect esstabish!\n");
	return sockfd;
}




int main(int argc, char const *argv[])
{
	int sockfd;
	char msg[MAXSIZE];
	int n;
	sockfd = startup(argc, argv);
	// str_cli(stdin, sockfd);
	str_add(sockfd);
	close(sockfd);
	exit(0);
}
