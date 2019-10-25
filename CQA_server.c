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

	pthread_t sniffer_thread;

	return 0;
}
