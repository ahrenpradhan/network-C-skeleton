#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<pthread.h>
int requestList[20];
int lastList[20];
int queue[20];
int ptr=0,ind=-1;
int client = -1;
int sem = -1;
int temp1;
int request(int x);
void *thread2(){
	int x;
	while(1){
		if(sem==1){
			x = queue[ptr];		//retriving the process at the top of the queue
			if(x!=-1){
				queue[ptr] = -1;	//deallocating the calue of that position of the queue
				ptr = (++ptr)%20;	//similar to popping operation
				lastList[x]++;		//updating the last queue
			}
			sem--;
		}
	}
	pthread_exit(NULL);
	return;
}

int request(int x){
	requestList[x]++;
	queue[(++ind)%20] = x;
}

void display(){
	int i;
	printf("\n");
	for(i=0;i<20;i++){
		printf("%d  ",requestList[i]);
	}
	printf("\n");
	for(i=0;i<20;i++){
		printf("%d  ",lastList[i]);
	}
	printf("\n");
	for(i=0;i<20;i++){
		printf("%d  ",queue[i]);
	}
}

void *connection_handler(void * socket_desc){
	int sock = *(int *) socket_desc;
	int n,i;
	char buffer[100];
	int c_num = ++client;
	while(1){
		strcpy(buffer,"");
		recv(sock,buffer,sizeof(buffer),0);
		n = atoi(buffer);
		switch(n){
				/*client requests*/
			case 1: printf("\n-----------------------------------------------------\nCalled by process %d...........",c_num);
				request(c_num);
				display();
				for(i=0;i<20;i++){
					strcpy(buffer,"");
					sprintf(buffer,"%d",requestList[i]);
					send(sock,buffer,sizeof(buffer),0);
				}
				for(i=0;i<20;i++){
					strcpy(buffer,"");
					sprintf(buffer,"%d",lastList[i]);
					send(sock,buffer,sizeof(buffer),0);
				}
				break;
				/*user defined instruction for the server to execute*/
			case 2: sem++;
				for(i=0;i<20;i++){
					strcpy(buffer,"");
					sprintf(buffer,"%d",requestList[i]);
					send(sock,buffer,sizeof(buffer),0);
				}
				for(i=0;i<20;i++){
					strcpy(buffer,"");
					sprintf(buffer,"%d",lastList[i]);
					send(sock,buffer,sizeof(buffer),0);
				}
				break;
				/*do nothing but display*/
			case 3: printf("\n*****************************************");
				for(i=0;i<20;i++){
					strcpy(buffer,"");
					sprintf(buffer,"%d",requestList[i]);
					send(sock,buffer,sizeof(buffer),0);
				}
				for(i=0;i<20;i++){
					strcpy(buffer,"");
					sprintf(buffer,"%d",lastList[i]);
					send(sock,buffer,sizeof(buffer),0);
				}
				display();
				break;
			default:
				free(socket_desc);
				close(sock);
				pthread_exit(NULL);
				return;
		}
	}
}

int main(int agrc,char *argv[]){
	int listenfd,client_sock,c, n,i,*new_sock;
	struct sockaddr_in servaddr,cliaddr;
	char buffer[100];
	pthread_t t2;
	for(i=0;i<20;i++){
		requestList[i]=-1;
		lastList[i]=-1;
		queue[i]=-1;
	}
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	printf("\nBINDED\n");
	listen(listenfd,10);
	printf("\nLISTEN\n");

	printf("\nWaiting for incoming connection..........\n");
	pthread_create(&t2,NULL,thread2,NULL);
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
