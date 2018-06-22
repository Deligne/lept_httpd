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

int get_msg(int sockfd) {
	char buf[MAXSIZE];
	int n;
	n = recv(sockfd, buf, MAXSIZE, 0);
	buf[n] = '\0';
	printf("get: %s\n", buf);
	return 1;
}

int send_msg(int sockfd, char *msg) {
	int n;
	n = write(sockfd, msg, strlen(msg));
	printf("send: %s\n", msg);
}


int main(int argc, char const *argv[])
{
	int sockfd;
	char msg[MAXSIZE];
	sockfd = startup(argc, argv);
	while(1) {
		fgets(msg, 1024, stdin);
		write(sockfd, msg, strlen(msg));
		printf("send: %s\n", msg);
		get_msg(sockfd);
	}
	close(sockfd);
	return 0;
}