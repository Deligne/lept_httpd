#include "lept_httpd.h"


void Listen(int fd, int backlog) {
	char *ptr;
	if((ptr = getenv("listennum")) != NULL) {
		backlog = atoi(ptr);
	}

	if(listen(fd, backlog) == -1) {
		err_sys("listen");
	}
}

int startup(u_short port = 8888) {
	int listenfd = 0;
	struct sockaddr_in socket_addr;
	memset(&socket_addr, 0, sizeof(socket_addr));
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(port);
	socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);//ANADDR_ANY = 0;
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) ==-1)
		err_sys("socket");
	if(bind(listenfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr))) 
		err_sys("bind");
	Listen(listenfd, 5);
	socklen_t len = sizeof(socket_addr);
	return listenfd;
}

void echo(int connfd) {
	char buf[MAXSIZE];
	while(1) {
		int n = read(connfd,buf, MAXSIZE);
		if(n > 0) {
			buf[n] = '\0';
			printf("msg: %s\n", buf);
		}
		write(connfd, buf, n);
	}
}

void header(int connfd) {
	char buf[MAXSIZE];
	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Server:lept_httpd\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Content-Type:text/html");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "Referrer");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);
}

void hello_world(int connfd) {
	char msg[] = "<h1>hello visitor</h1>";
	send(connfd, msg, strlen(msg), 0);
}

  
/*返回客户端404错误信息*/  
void not_found(int client)  
{  
    char buf[1024];  
    sprintf(buf,"HTTP/1.0 404 NOT FOUND\r\n");  
    send(client,buf,strlen(buf),0);  
    sprintf(buf,"SERVER_STRING");  
    send(client,buf,strlen(buf),0);  
    sprintf(buf,"Content-Type: text/html\r\n");  
    send(client,buf,strlen(buf),0);  
    sprintf(buf,"\r\n");  
    send(client,buf,strlen(buf),0);  
    sprintf(buf,"<HTML><TITLE>Not Found</TITLE>\r\n");  
    send(client,buf,strlen(buf),0);  
    sprintf(buf,"<BODY><P>The server could not fulfill\r\n");  
    send(client,buf,strlen(buf),0);  
    sprintf(buf,"your request because the resource specified\r\n");  
    send(client,buf,strlen(buf),0);  
    sprintf(buf,"is unavailable or nonexistent.\r\n");  
    send(client,buf,strlen(buf),0);  
    sprintf(buf,"</BODY></HTML>\r\n");  
    send(client,buf,strlen(buf),0);  
}  

  
/*告知客户端请求有错误*/  
void bad_request(int client)  
{  
    char buf[1024];  
    /*将字符存入缓冲区域,再通过send函数发送给客户端*/  
    sprintf(buf,"HTTP/1.0 400 BAD REQUEST\r\n");  
    send(client,buf,sizeof(buf),0);  
    sprintf(buf,"Content-type: text/html\r\n");   
    send(client,buf,sizeof(buf),0);  
    sprintf(buf,"\r\n");  
    send(client,buf,sizeof(buf),0);  
    sprintf(buf,"<P>Your browser sent a bad request, ");  
    send(client,buf,sizeof(buf),0);  
    sprintf(buf,"such as a POST without a Content-Length.\r\n");  
    send(client,buf,sizeof(buf),0);  
      
}  

void signal_handler(int signo) {

}

unsigned int get_line(int fd, char *buf, unsigned int size) {
	int i = 0;
	char c = '\0';
	int n;
	while((i < size - 1) && (c != '\n')) {
		n = recv(fd, &c, 1, 0);
		if(n > 0) {
			if(n = '\r') {
				n = recv(fd, &c, 1, 0);
				if((n > 0) && (c =='\n')) 
					recv(fd, &c, 1, 0);
				else 
					c = '\n';
			}
		}
		buf[i] = c;
		i++;
	}
	buf[i] = '\0';
	return i;
}


int accept_request(int listenfd) {
	struct sockaddr_in client_addr;
	struct sockaddr_in temp_addr;
	socklen_t len;
	int connfd;
	pid_t pid;
	len = sizeof(client_addr);
	for(;;) {
		connfd = accept(listenfd, (struct sockaddr *) &client_addr, &len);//blocked
		if((pid = fork()) == 0) {
			char buf[MAXSIZE];
			close(listenfd);
			if((getpeername(connfd, (struct sockaddr *) &temp_addr, &len)) == 0) {
				int i = htonl(client_addr.sin_addr.s_addr);
    			printf("pid:%d accept connection from  ip: %d.\n", getpid(), i);
    			printf("pid:%d accept connection from  ip: %d.\n", getpid(), client_addr.sin_addr.s_addr);
			}
			echo(connfd);
			exit(0);
		}
		close(connfd);
	}
}


int main(int argc, char const *argv[])
{
	int sockfd;
	sockfd = startup(80);
	accept_request(sockfd);
	return 0;
}