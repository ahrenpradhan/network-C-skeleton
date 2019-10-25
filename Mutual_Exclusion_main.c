#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

int semaphore = 1;
int p = 1;

void * client_handler(void * socket_desc){

}

void * connection_handler(void * socket_desc){
	int process = p++;
	int sock = *(int *) socket_desc;
	int read_size,n;
	char *message,buffer[2000];
	strcpy(buffer,"");
	recv(sock,buffer,sizeof(buffer),0);
	printf("\nNo. of processes to execute : %s",buffer);		/* enter the number of precesses */
	n = atoi(buffer);
	while(n){
		strcpy(buffer,"");
		if(wait(semaphore)){					/*reserving the semaphore*/
			strcpy(buffer,"Reserved the server for itself");
			send(sock,buffer,sizeof(buffer),0);
			printf("\nProcess reserved by process: %d\n",process);
			n--;
			sleep(5);
			signal(semaphore);					/*releasing the semaphore*/
		}
		else{
			strcpy(buffer,"Reserved by some other process");
			send(sock,buffer,sizeof(buffer),0);		/*SENDING*/
			sleep(3);
		}
	}
	printf("\n***************************Process %d has been executed***************************\n",process);
	strcpy(buffer,"Process has been executed %d....");
	send(sock,buffer,sizeof(buffer),0);		/*SENDING*/

	strcpy(buffer,"0");
	send(sock,buffer,sizeof(buffer),0);

	fflush(stdout);
	free(socket_desc);
	close(sock);
	pthread_exit(NULL);
	return;
}

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

	if(vfork()){
		printf("\nWaiting for incoming connection..........\n");

		c = sizeof(cliaddr);
		printf("\n*****************************");
		printf("\n*****************************");
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
	}
	else
	return 0;
}
