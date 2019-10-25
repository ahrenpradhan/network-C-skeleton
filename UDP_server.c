#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
int main(int agrc,char *argv[]){
	int listenfd;
	struct sockaddr_in servaddr,clientaddr;
	
	char *banner="Hello TCP client";
	char buffer[100];

	listenfd = socket(AF_INET,SOCK_DGRAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	printf("\nBINDED\n");	

	recvfrom(listenfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
	printf("\nRECIEVED\n");
	printf("\nThe message recieved is : %s\n\n",buffer);

	sendto(connfd,banner,sizeof(banner),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
	printf("\nSENT\n");
/*
	*/
	close(listenfd);
	return 0;
}
