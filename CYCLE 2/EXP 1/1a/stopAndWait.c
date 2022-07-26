#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#include "head.h"

int frames[10];

int sockfd;
struct sockaddr_in server;
int addrlen;

pthread_t t;

clock_t clockStart,clockEnd;
double timeTaken;

void initialize(){
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0){
		perror("Socket creation failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Socket created successfully\n");
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(port);
	addrlen=sizeof(server);
	
	if (connect(sockfd,(struct sockaddr*)&server,(socklen_t)addrlen)<0){
		perror("Connection to server failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Connection to server success\n");
} 

void flush(){
	
	fflush(stdin);
	fflush(stdout);
}

void* stopAndWait(){
	
	// char *msg=(char *)malloc(sizeof(char)*100);
    char msg[100];
	char *retmsg=(char *)malloc(sizeof(char)*100);
	
	int i=0;
	while (1){
		sprintf(msg,"%d",frames[i++]);
        
        printf("Sending frame: %s\n",msg);
		send(sockfd,msg,sizeof(char)*strlen(msg),0);
        
		clockStart=clock();
      	recv(sockfd,retmsg,sizeof(char)*100,0);
		clockEnd=clock();
		
		timeTaken=(double)((double)clockEnd-(double)clockStart)/CLOCKS_PER_SEC;
		printf("TIME: %lf\n",timeTaken);
		
		if (timeTaken>3){
			i--;
			printf("TIME EXCEEDED!!\n");
			printf("Resending...\n");
		}
        else{
		printf("Message from server: %s\n",retmsg);
		}

        if (i==10) break;
	}
	
}

void main(){
	initialize();
	
	for (int i=0;i<10;i++)	frames[i]=i;
	
	pthread_create(&t,NULL,stopAndWait,NULL);
	pthread_join(t,NULL);
}

