#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define port 8080

void main()
{
	int serverfd;
	struct sockaddr_in server,client;
	int addrlen=sizeof(server);
	
	serverfd=socket(AF_INET,SOCK_DGRAM,0);
	
	if (serverfd<0)
		printf("Socket creation failed\n");
	else
		printf("Socket created successfully\n");
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(port);

	
	if (bind(serverfd,(struct sockaddr*)&server,addrlen)<0)
		printf("Bind failed\n");
	else
		printf("Bind successfull\n");
	
	char *retmsg;
	retmsg=(char *)malloc(sizeof(char)*110);
	
	recvfrom(serverfd,retmsg,sizeof(char)*1000,MSG_WAITALL,(struct sockaddr*)&client,&addrlen);
	
	printf("%s\n",retmsg);
	
	sendto(serverfd,"HELLO WORLD...",sizeof(char)*50,MSG_CONFIRM,(struct sockaddr*)&client,addrlen);
}

