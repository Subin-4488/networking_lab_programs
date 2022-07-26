#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "head.h"

int sockfd,clientfd;
struct sockaddr_in server,client;
int addrlen;

pthread_t t;

void initialize(){
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0){
		perror("Socket creation failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Socket created successfully\n");
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(port);
	addrlen=sizeof(server);
	
	if (bind(sockfd,(struct sockaddr*)&server,(socklen_t)addrlen)<0){
		perror("Binded Unsuccessful\n");
		exit(EXIT_FAILURE);
	}
	printf("Bind success\n");
	
	listen(sockfd,5);
	clientfd=accept(sockfd,(struct sockaddr*)&client,(socklen_t*)&addrlen);
	if (clientfd<0){
		perror("Client Accept unsuccessfull\n");
		exit(EXIT_FAILURE);
	}
	printf("Client accepted successfully\n");
} 

void flush(){
	
	fflush(stdin);
	fflush(stdout);
}

void* receiveAndAcknowledge(){
	
	char *msg=(char *)malloc(sizeof(char)*100);
	char *retmsg=(char *)malloc(sizeof(char)*100);

	msg[0]='\0';
	
	strcpy(retmsg,"ACKNOWLEDGED !!");
	
	while (1){
        printf("Receiving...\n");
        sleep(2);

		recv(clientfd,msg,sizeof(char)*100,0);  

		if (msg[0]=='\0') exit(EXIT_SUCCESS);

		printf("Received frame: %s\n",msg); flush();

		printf("Sending acknowledgement...\n"); flush();
        sleep(1);
		
		send(clientfd,retmsg,sizeof(char)*strlen(retmsg),0); flush();
		msg[0]='\0';
	}
	
}

void main(){
	initialize();

	pthread_create(&t,NULL,receiveAndAcknowledge,NULL);
	pthread_join(t,NULL);
}