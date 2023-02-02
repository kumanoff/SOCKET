#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int Socket(int domain, int type, int protocol);
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int Socket(int domain, int type, int protocol){// "обёртка" wrapper-function, for printing errors during making socket
	int res = socket(domain, type, protocol);
	if (res == -1){
		perror("socket failure");
		exit(EXIT_FAILURE);
	}
	return res;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
	int res = bind(sockfd, addr, addrlen);
	if (res == -1){
		perror("binding failure");
		exit(EXIT_FAILURE);
	}
}

void Listen(int sockfd, int backlog){
	int res = listen(sockfd, backlog);
	if (res == -1){
		perror("listenig failed");
		exit(EXIT_FAILURE);
	}
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
	int res = accept(sockfd, addr, addrlen);
	if (res == -1){
		perror("accept is fucked up"); 
		exit(EXIT_FAILURE);
	}
	return res;
}

int main(){
	int server = Socket(AF_INET, SOCK_STREAM, 0);//descriptor of socket to variable

	struct sockaddr_in adr = {0};//initilise fields from struct
	adr.sin_family = AF_INET;// "семейство" адресов
	adr.sin_port = htons(34543);//port of listening 
	//send it to bind 
	Bind(server, (struct sockaddr *) &adr, sizeof adr);//give to binding our server from beggining of main 

	Listen(server, 5);//till 5 clients can wait in order 

	socklen_t addrlen = sizeof adr;//size of variable "adr"
	int fd = Accept(server, (struct sockaddr *) &adr, &addrlen);//we rewriting variable 'adr' by data from client; fd - file descriptor
	
	ssize_t nread;
	char buf[256];//buffer - 256 cells  
	nread = read(fd, buf, 256);
	if (nread == -1){
		perror("reading is fucked up");
		exit(EXIT_FAILURE);
	}
	if (nread == 0){
		printf("end of file occured dude\n");
	}
	write(STDOUT_FILENO, buf, nread);//give readed message from server to console
	write(fd, buf, nread);

	char text_file[100];
    FILE *k = fopen("probnik.txt", "r"); // файл из которого читаем значение
    fscanf(k,"%s",text_file);

    int len = sizeof(text_file);
    sendto(fd ,(char *)"<body>" text_file " </body>", len,0,(struct sockaddr *) &adr, sizeof(adr));

    fclose(k);
    printf("%s \n", text_file);// a - address of variable 
	sleep(12);
	close(fd);
	close(server); 

	return 0;
}