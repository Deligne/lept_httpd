#include "getaddr.h"


int main(int argc, char const *argv[])
{
	int sockfd, n;
	char recvline[MAXSIZE];
	socklen_t len;
	struct sockaddr_storage ss;

	if(argc != 3) {
		err_sys("usage: filename <hostname/ipadress> <service/port>");
	}

	sockfd = tcp_connect(argv[1], argv[2]);

	len = sizeof(ss);

	getpeername(sockfd, (struct sockaddr *) &ss, &len);
	printf("%d\n", ss.ss_family);
	while((n = read(sockfd, recvline, sizeof(recvline))) > 0) {
		recvline[n] = 0;
		fputs(recvline, stdout);
	}
	
	exit(0);
}