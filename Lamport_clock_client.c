#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>

int clock = 0;
void clockupdate(int x);

int main(int agrc,char * argv[]){

	int connfd, n;
	struct sockaddr_in cliaddr;
	//socklen_t clilen;
	char buffer[100]="Message is recieved at the client side";

	connfd = socket(AF_INET,SOCK_STREAM,0);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cliaddr.sin_port = htons(atoi(argv[1]));

	connect(connfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	printf("\nCONNECTED\n");
	clockupdate(0);

	/*recv(connfd,buffer,sizeof(buffer),0);
	printf("\nRECIEVED\n");
	printf("\n%s\n\n",buffer);
	clockupdate(0);*/
	printf("\n*****************************");
	printf("\n*****************************");
	while(1){
		printf("\n--------------------------------------------------");

		strcpy(buffer,"");
		printf("\nEnter the message you want to send : ");
		scanf("%s",buffer);
		send(connfd,buffer,sizeof(buffer),0);
		clockupdate(0);

		strcpy(buffer,"");
		sprintf(buffer,"%d",clock);
		send(connfd,buffer,sizeof(buffer),0);		/*SENDING THE CLOCK VALUE*/
		printf("\nSENT\t\t\tThe clock sent is : %s",buffer);
		clockupdate(0);

		strcpy(buffer,"");
		recv(connfd,buffer,sizeof(buffer),0);		/*RECIEVING THE CLOCK VALUE*/
		printf("\n\nRECIEVED\t\tThe clock recieved is : %s",buffer);
		n = atoi(buffer);
		clockupdate(n);
	}
	return 0;
}
void clockupdate(int x){
	printf("\nclock updated from %d to ",clock);
	if(x>clock)
		clock = x;
	clock++;
	printf("%d",clock);
}
