#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
int main(int agrc,char * argv[]){
	int ID;
	int connfd, n;
	struct sockaddr_in cliaddr;
	//socklen_t clilen;
	char buffer[2000];

	connfd = socket(AF_INET,SOCK_STREAM,0);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = inet_addr("192.168.10.202");
	cliaddr.sin_port = htons(atoi(argv[1]));
	connect(connfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	printf("\nCONNECTED\n");
	
	recv(connfd,buffer,sizeof(buffer),0);
	ID = atoi(buffer);
	printf("The ID is %d\n\n\nDo you want an Election (YES/NO) : ",ID);
	scanf("%s",buffer);
	send(connfd,buffer,sizeof(buffer),0);
	while(1){
		strcpy(buffer,"");
		recv(connfd,buffer,sizeof(buffer),0);
		if(strcmp(buffer,"Election")==0){
			strcpy(buffer,"");
			strcpy(buffer,"OK");
			send(connfd,buffer,sizeof(buffer),0);
			recv(connfd,buffer,sizeof(buffer),0);
		}
		else if(strcmp(buffer,"Next")==0){
			strcpy(buffer,"");
			strcpy(buffer,"YES");
			send(connfd,buffer,sizeof(buffer),0);
			recv(connfd,buffer,sizeof(buffer),0);
		}
		else{
			printf("Message is : %s",buffer);
		}
	}
	return 0;
}
