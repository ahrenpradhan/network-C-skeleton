#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<pthread.h>
void *connection_handler(void * socket_desc);

int process_list[100] = -1;
int p=0;

int main(int agrc,char *argv[]){
	int listenfd,client_sock,c, n,*new_sock;
	struct sockaddr_in servaddr,cliaddr;
	char *banner="Hello TCP client";
	char buffer[100];
	
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	printf("\nBINDED\n");
	listen(listenfd,0);
	printf("\nLISTEN\n");

	printf("\nWaiting for incoming connection..........\n");

	c = sizeof(cliaddr);
	while( client_sock = accept(listenfd,(struct sockaddr *)&cliaddr,(socklen_t *)&c) ){
		printf("\nACCEPTED\n");
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		if(pthread_create(&sniffer_thread,NULL,connection_handler,(void *)new_sock)<0){
			perror("couldn't create thread");
			return 1;
		}
		printf("\nHandler Assigned\n");
	}
	if(client_sock<0){
		perror("accept failed");
		return 1;
	}
	return 0;
}



void *connection_handler(void * socket_desc){
	p++;
	int sock = *(int *) socket_desc;
	int read_size,i;
	char client_message[2000];
	char temp[10];
	int process;
	recv(sock,client_message,sizeof(client_message),0);
	process = atoi(client_message);		//process recieved
	process_list[process] = sock;		//socket stored globally
	while((read_size = recv(sock,client_message,sizeof(client_message),0))>0){

		if(strcmp(client_message,"ELECTION")==0){			//For election
			strcpy(client_message,"");
			for(i=0;i<100;i++){
				if(process_list[i]!=-1){
					strcpy(client_message,"ALIVE");
					send(process_list[i],client_message,sizeof(client_message),0);
					strcpy(client_message,"");
					strcpy(client_message,"I am alive__process ");
					temp = atoi(i);
					strcat(client_message,temp);
					send(process_list[i],client_message,sizeof(client_message),0);
				}
			}
		}

		if(strcmp(client_message,"")==0){
			
		}
	}
	if(read_size==0){
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size==-1){
		perror("recv failed");
	}
	free(socket_desc);
	close(sock);
	pthread_exit(NULL);
	return;
}













