#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define port 8080

void main()
{
	int clientfd,serverfd;
	struct sockaddr_in server;
	int addrlen=sizeof(server);
	
	clientfd=socket(AF_INET,SOCK_STREAM,0);
	
	if (clientfd<0)
		printf("Socket creation failed\n");
	else
		printf("Socket created successfully\n");
	
	char msg[1000]="HELLO WORLD...";
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("172.20.31.247");
	server.sin_port=htons(port);
	
	serverfd=connect(clientfd,(struct sockaddr*)&server,addrlen);
	if (serverfd<0)
		printf("Connect failed\n");
	else
		printf("Server connected\n");
	
	char *retmsg;
	retmsg=(char *)malloc(sizeof(char)*1000);
	
	send(clientfd,msg,sizeof(char)*1000,0);
	
	if (recv(clientfd,retmsg,sizeof(char)*1000,0))
	{
		printf("%s\n",retmsg);
	}
	else
	{
		printf("Receive failed\n");
	}
	close(clientfd);
}
