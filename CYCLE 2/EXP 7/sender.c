//sender

#include "head.h"

int sockfd;
struct sockaddr_in server;

void initialize()
{
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0)
	{
		perror("socket creation failed\n");
		exit(EXIT_FAILURE);
	}
	printf("socket created successfully\n");
	
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_family=AF_INET;
	server.sin_port=htons(port);
	
	if (connect(sockfd,(struct sockaddr*)&server,(socklen_t)addrlen)<0)
	{
		perror("connection to server failed\n");
		exit(EXIT_FAILURE);
	}
	
}

void sendPackets()
{
	char msg[50]="packet";
	while (1)
	{	
		send(sockfd,msg,sizeof(char)*50,0);
		sleep(1);
	}
}

void main()
{
	initialize();
	
	sendPackets();
}