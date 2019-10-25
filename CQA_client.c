#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<pthread.h>

int main(int agrc,char * argv[]){

	int connfd, n;
	int a, b;
	struct sockaddr_in cliaddr;
	char buffer[100];

	connfd = socket(AF_INET,SOCK_STREAM,0);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliaddr.sin_port = htons(atoi(argv[1]));

	connect(connfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	printf("\nCONNECTED\n");

	printf("Enter the value of a : ");
	scanf("%d",&a);
	sprintf(buffer,"%d",a);
	send(connfd,buffer,sizeof(buffer),0);

	printf("Enter the value of b : ");
	scanf("%d",&b);
	strcpy(buffer,"");
	sprintf(buffer,"%d",b);
	send(connfd,buffer,sizeof(buffer),0);

	
	printf("\nRECIEVED\n");
	strcpy(buffer,"");
	recv(connfd,buffer,sizeof(buffer),0);
	printf("\n%s\n\n",buffer);

	return 0;
}
