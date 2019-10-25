#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>

int clock = 0;
void clockupdate(int x);


int main(int agrc,char *argv[]){
	int listenfd,connfd, n;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t clilen;
	char buffer[100] = "Hello from the other side...";
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	clockupdate(0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	printf("\nBINDED\n");
	clockupdate(0);

	listen(listenfd,0);
	printf("\nLISTEN\n");
	clockupdate(0);

	clilen = sizeof(cliaddr);

	connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
	printf("\nACCEPTED\n");
	clockupdate(0);

	send(connfd,buffer,sizeof(buffer),0);
	clockupdate(0);
	printf("\n*****************************");
	printf("\n*****************************");
	while(1){
		printf("\n--------------------------------------------------");
		
		/*
		*/
		strcpy(buffer,"");
		recv(connfd,buffer,sizeof(buffer),0);
		printf("\nText Message : %s",buffer);
		clockupdate(0);

		strcpy(buffer,"");
		recv(connfd,buffer,sizeof(buffer),0);		/*RECIEVEING THE CLOCK VALUE*/
		printf("\nRECIEVED\t\tThe clock recieved is : %s",buffer);

		n = atoi(buffer);
		clockupdate(n);
		strcpy(buffer,"");
		sprintf(buffer,"%d",clock);
		send(connfd,buffer,sizeof(buffer),0);		/*SENDING THE CLOCK VALUE*/
		printf("\n\nSENT\t\t\tThe clock sent is : %s",buffer);
		clockupdate(0);
	}
/*
	*/
	return 0;
}

void clockupdate(int x){
	printf("\nclock updated from %d to ",clock);
	if(x>clock)
		clock = x;
	clock++;
	printf("%d",clock);
}
