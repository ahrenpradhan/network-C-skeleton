#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
int main(int agrc,char * argv[]){

	int connfd, n;
	struct sockaddr_in cliaddr;
	//socklen_t clilen;
	char * message="text from client";
	char buffer[2000];

	connfd = socket(AF_INET,SOCK_STREAM,0);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliaddr.sin_port = htons(atoi(argv[1]));
	//clilen = sizeof(cliaddr);

	connect(connfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	printf("\nCONNECTED\n");
	send(connfd,message,sizeof(message),0);
	printf("\nSENT\n");
	sleep(100);
	recv(connfd,buffer,sizeof(buffer),0);
	printf("\nRECIEVED\n");
	printf("\n%s\n\n",buffer);

	return 0;
}
