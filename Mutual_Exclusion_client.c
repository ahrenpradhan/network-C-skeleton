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
	char buffer[100]="Message is recieved at the client side";

	connfd = socket(AF_INET,SOCK_STREAM,0);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliaddr.sin_port = htons(atoi(argv[1]));

	connect(connfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	printf("\nCONNECTED\n");

	printf("\nEnter the number of processes to be executed in the server.... : ");
	scanf("%s",buffer);
	send(connfd,buffer,sizeof(buffer),0);
	strcpy(buffer,"");
	printf("\n*****************************");
	printf("\n*****************************");
	n = 0;
	while(1){
		n++;
		strcpy(buffer,"");
		recv(connfd,buffer,sizeof(buffer),0);		/*RECIEVING*/
		if(strcmp(buffer,"Reserved the server for itself")==0){
			sleep(5);
			printf("\n%d  Message from Server...: %s",n,buffer);
		}
		if(strcmp(buffer,"Reserved by some other process")==0){
			sleep(3);
			printf("\n%d  Message from Server...: %s",n,buffer);
		}
		if(strcmp(buffer,"0")==0){
			return 0;
		}
		strcpy(buffer,"");
	}
	return 0;
}
