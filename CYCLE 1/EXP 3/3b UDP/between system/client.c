#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define port 8080

void main()
{
	int clientfd;
	struct sockaddr_in server,client;
	int addrlen=sizeof(server);
	
	clientfd=socket(AF_INET,SOCK_DGRAM,0);
	
	if (clientfd<0)
		printf("Socket creation failed\n");
	else
		printf("Socket created successfully\n");
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(port);
	
	char msg[100]="MESSAGE FROM SUBIN";
	
	sendto(clientfd,msg,sizeof(char)*100,MSG_CONFIRM,(struct sockaddr*)&server,addrlen);
	
	char *retmsg;
	retmsg=(char *)malloc(sizeof(char)*110);
	
	recvfrom(clientfd,retmsg,sizeof(char)*200,MSG_WAITALL,(struct sockaddr*)&client,&addrlen);
	printf("%s\n",retmsg);
}
