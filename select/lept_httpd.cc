#include "lept_httpd.h"

void str_cli_select(FILE *fp, int sockfd) {
	int maxfdpl, stdineof;
	fd_set rset;
	FD_ZERO(&rset);
	char buf[MAXSIZE];
	int n;
	stdineof = 0;
	for(;;) {
		if(stdineof == 0) {
			FD_SET(fileno(fp), &rset);
		}
		FD_SET(sockfd, &rset);
		maxfdpl = max(fileno(fp), sockfd);
		select(maxfdpl, &rset, NULL, NULL, NULL);
		if(FD_ISSET(sockfd, &rset)) {
			if((n == read(sockfd, buf, sizeof(buf)))) {
				if(stdineof == 1)
					return ;
				else 
					err_sys("str_cli_select: server terminated permaturely\n");
			}
			write(fileno(fp), buf, n);
		}

		if(FD_ISSET(fileno(fp), &rset)) {
			if(n == read(fileno(fp), buf, sizeof(buf)) == 0) {
				stdineof == 1;
				shutdown(sockfd, SHUT_WR);
				FD_CLR(fileno(fp), &rset);
				continue;
			}
			write(sockfd, buf, n);
		}
	}
}



int main(int argc, char const *argv[])
{
	int i, sockfd, connfd, listenfd, maxi, maxfd;
	int nready, client[FD_SETSIZE];
	ssize_t n;
	struct sockaddr_in cliaddr, servaddr;
	fd_set allset, rset;
	char buf[MAXSIZE];
	socklen_t clilen;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&cliaddr, sizeof(cliaddr));
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
	int LISTENQ = atoi(getenv("LISTENQ"));
	listen(listenfd, LISTENQ);

	maxfd = listenfd;
	maxi = -1;
	for(i = 0; i < FD_SETSIZE; i++) {
		client[i] = -1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for(; ;) {
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

		//new client
		if (FD_ISSET(listenfd, &rset)) {
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

			//add to client[]
			for(i = 0; i < FD_SETSIZE; i++) {
				if(client[i] < 0) {
					client[i] = connfd;
					break;
				}
			}

			if(i == FD_SETSIZE)
				err_sys("to much clients");
			FD_SET(connfd, &allset);

			if(connfd > maxfd)
				maxfd = connfd;

			if(i > maxi)
				maxi = i;

			if(--nready <= 0)
				continue;
		}

		//check for data
		for(i = 0; i <= maxi; i++) {
			if((sockfd = client[i]) < 0)
				continue;
			if(FD_ISSET(sockfd, &rset)) {
				if((n = read(sockfd, buf, sizeof(buf))) == 0) {
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else {
					write(sockfd, buf, n);
				}

				if(--nready <= 0)
					break;
			}
		}
	}
	return 0;
}


