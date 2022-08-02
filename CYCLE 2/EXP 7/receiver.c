//receiver

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

void receivePackets()
{
	int size=1000;
	char msg[1000];
	
	while (1)
	{	
		recv(sockfd,msg,sizeof(char)*50,0);
		printf("RECEIVED FROM SERVER: %s\n",msg);
	}
}

void main()
{
	initialize();
	
	receivePackets();
}