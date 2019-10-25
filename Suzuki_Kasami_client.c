#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>

int requestList[20];
int lastList[20];

void display(){
	int i;
	printf("\n");
	for(i=0;i<20;i++){
		printf("%d\t",requestList[i]);
	}
	printf("\n");
	for(i=0;i<20;i++){
		printf("%d\t",lastList[i]);
	}
}

int main(int agrc,char * argv[]){

	int connfd, n, i;
	struct sockaddr_in cliaddr;
	char buffer[100];

	connfd = socket(AF_INET,SOCK_STREAM,0);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliaddr.sin_port = htons(atoi(argv[1]));

	connect(connfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	printf("\nCONNECTED\n");
	while(1){
		strcpy(buffer,"");
		printf("\nInstruction........ : ");
		scanf("%s",buffer);
		send(connfd,buffer,sizeof(buffer),0);
		printf("\nSENT\n");
		for(i=0;i<20;i++){
			strcpy(buffer,"");
			recv(connfd,buffer,sizeof(buffer),0);
			requestList[i] = atoi(buffer);
		}
		for(i=0;i<20;i++){
			strcpy(buffer,"");
			recv(connfd,buffer,sizeof(buffer),0);
			lastList[i] = atoi(buffer);
		}
		display();
	}
	printf("\nRECIEVED\n");
	printf("\n%s\n\n",buffer);

	return 0;
}
