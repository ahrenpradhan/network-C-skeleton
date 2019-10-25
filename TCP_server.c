#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>

int main(int agrc,char *argv[]){
	int listenfd,connfd, n;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t clilen;
	char *banner="Hello TCP client";
	char buffer[100];

	listenfd = socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	printf("\nBINDED\n");
	listen(listenfd,0);
	printf("\nLISTEN\n");
	clilen = sizeof(cliaddr);

	connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
	printf("\nACCEPTED\n");
	recv(connfd,buffer,sizeof(buffer),0);
	printf("\nRECIEVED\n");
	printf("\nThe message recieved is : %s\n\n",buffer);

	send(connfd,banner,sizeof(banner),0);
	printf("\nSENT\n");
/*
	*/
	return 0;
}
