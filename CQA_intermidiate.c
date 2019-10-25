#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<pthread.h>

//arrays of servers and clients
struct node{
	int *sock;
	int cpu;
};
struct node servers[100];
//maxcount of servers and clients
int server_count = -1;



int select_server(){
	int i, max=-10;
	for(i=0;i<server_count;i++){
		if(servers[i]->cpu){
			max = i;
		}
	}
	max = servers[max]->cpu;
	servers[max]->cpu = 0;
	return max;
}

void *connection_handler(void * socket_desc){
	int sock = *(int *) socket_desc;
	int current_server_id;
	int n, i, id;
	char buffer[100];
	strcpy(buffer,"");
	recv(sock,buffer,sizeof(buffer),0);
	if(strcmp(buffer,"s")==0){	//if server sends a request
		server_count++;
		current_server_id = server_count;
		servers[server_count]->sock = &sock;
		while(1){
			strcpy(buffer,"");
			recv(sock,buffer,sizeof(buffer),0);
			servers[current_server_id]->cpu = atoi(buffer);
		}
	}
	else{				//if client sends a request 
		while(1){
			id = select_server();	//selecting the best server
			strcpy(buffer,"");
			recv(sock,buffer,sizeof(buffer),0);		//receiving 'a' from client
			send(servers[id]->sock,buffer,sizeof(buffer),0)//sending 'a' from server
			strcpy(buffer,"");
			recv(sock,buffer,sizeof(buffer),0);		//receiving 'b' from client
			send(servers[id]->sock,buffer,sizeof(buffer),0)	//sending 'b' from server
			
			recv(sock,buffer,sizeof(buffer),0);		//receiving solution from server
			send(sock,buffer,sizeof(buffer),0);		//sending result back to client
		}		
	}
}

int main(int agrc,char *argv[]){
	int listenfd,client_sock,c, n,i,*new_sock;
	struct sockaddr_in servaddr,cliaddr;
	char buffer[100];
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	printf("\nBINDED\n");
	listen(listenfd,10);
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
