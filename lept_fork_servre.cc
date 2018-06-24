#include "lept_httpd.cc"

int main(int argc, char const *argv[])
{
	int sockfd;
	sockfd = startup(80);
	accept_request(sockfd);
	return 0;
}