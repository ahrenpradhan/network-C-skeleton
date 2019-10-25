#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<pthread.h>

int p=0, check=0,xyz=-1;

void *thread2(){
	//halt(30000);
	printf("\nInitiate.......(1)\n");
	scanf("%d",&check);
	xyz = 0;
	while(1){
		if(check==0){
			xyz++;
			check=1;
		}
	}
	return;
}

void *connection_handler(void * socket_desc){
	int id = p++;
	int sock = *(int *) socket_desc;
	int n,i,ans;
	char buffer[100];
	int input[2] = {0,0};
	while(1){

		strcpy(buffer,"");
		recv(sock,buffer,sizeof(buffer),0);		//operation
		n = atoi(buffer);

		if(n==5){
			printf("\nInitiate.......(1)\n");
			check
			xyz = 0;
			while(1){
				if(check==0){
					xyz++;
					check=1;
				}
			}
			return;
		}
		strcpy(buffer,"");
		recv(sock,buffer,sizeof(buffer),0);		//input 1
		input[0] = atoi(buffer);
		strcpy(buffer,"");
		recv(sock,buffer,sizeof(buffer),0);		//input 2
		input[1] = atoi(buffer);
		while(1){
			if(check == 1 && xyz == p){
				switch(n){
					case 1:	//ADDITION
						ans = input[0] + input[1];
						break;
					case 2: //MULTIPLICATION
						ans  = input[1] * input[2];
						break;
					default:
						break;
				}
				strcpy(buffer,"");
				sprintf(buffer,"%d",ans);
				send(sock,buffer,sizeof(buffer),0);
				check = 0;
				return;
			}
		}
	}
}

int main(int agrc,char *argv[]){
	int listenfd,client_sock,c, n,i,*new_sock;
	struct sockaddr_in servaddr,cliaddr;
	char buffer[100];
	pthread_t t2;
	
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

	pthread_create(&t2,NULL,thread2,NULL);
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
