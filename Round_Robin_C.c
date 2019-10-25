#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>

int main(int agrc,char * argv[]){

	int connfd, n, i;
	struct sockaddr_in cliaddr;
	char buffer[100];

	connfd = socket(AF_INET,SOCK_STREAM,0);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliaddr.sin_port = htons(atoi(argv[1]));

	connect(connfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	printf("\nCONNECTED\nSelect an option\n\t1.ADDITION\n\t2.SUBTRACTION\n\t");
	strcpy(buffer,"");
	scanf("%s",buffer);
	send(connfd,buffer,sizeof(buffer),0);

	printf("INPUT 1:\t");
	strcpy(buffer,"");
	scanf("%s",buffer);
	send(connfd,buffer,sizeof(buffer),0);

	printf("INPUT 2:\t");
	strcpy(buffer,"");
	scanf("%s",buffer);
	send(connfd,buffer,sizeof(buffer),0);

	strcpy(buffer,"");
	recv(connfd,buffer,sizeof(buffer),0);
	printf("\nAnswer :\t%s\n\n",buffer);

	return 0;
}
