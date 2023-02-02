#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int Socket(int domain, int type, int protocol);
void Listen(int sockfd, int backlog);
void Connect(int sockfd, const struct sockaddr *add, socklen_t addrlen);

int Socket(int domain, int type, int protocol){// "обёртка" wrapper-function, for printing errors during making socket
	int res = socket(domain, type, protocol);
	if (res == -1){
		perror("socket failure");
		exit(EXIT_FAILURE);
	}
	return res;
}


void Listen(int sockfd, int backlog){
	int res = listen(sockfd, backlog);
	if (res == -1){
		perror("listenig failed");
		exit(EXIT_FAILURE);
	}
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
	int res = connect(sockfd, addr, addrlen);
	if (res == -1){
		perror("connect failed");
		exit(EXIT_FAILURE);
	}
}

void Inet_pton(int af, const char *src, void *dst){
	int res = inet_pton(af, src, dst);
	if (res == 0){
		printf("some weird error from manual\n");
	}
	if (res == -1){
		perror("connect failed");
		exit(EXIT_FAILURE);
	}
}

int main(){
	int fd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in adr = {0};
	adr.sin_family = AF_INET;
	adr.sin_port = htons(34543);
	Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);//addres that we are gonna connect
	Connect(fd, (struct sockaddr *) &adr, sizeof adr);//just connect

    char buf[1024];
    buf[0]=129;//0 бай   Asd
 	buf[1]=19; //1 байт
    buf[2]=5; //2 байт
    buf[3]=5;  //but ama work alone, actually((((
    sprintf(&buf[4],"Kumanev");//
    int len = 81;
    int xorbyte;
    len = sizeof(buf);
    for(int i = 0; i < len; i++) xorbyte = xorbyte ^ buf[i];
    printf("xor = %d\n", xorbyte);
    buf[81]=xorbyte;
    sendto(fd ,(char *)buf, len,0,(struct sockaddr *) &adr, sizeof(adr));//size of buf
    ssize_t nread;
    nread = read(fd, buf, 1024);
    if (nread == -1) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    if (nread == 0) {
        printf("EOF occured\n");
    }
    write(STDOUT_FILENO, buf, nread);
    sleep(7);
    close(fd);
    return 0;
}